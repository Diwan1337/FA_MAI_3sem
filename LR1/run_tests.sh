#!/usr/bin/env bash
# run_tests.sh — жёсткие тесты для ЛР1 (C99): task1..task8
# Режимы: fast|full|diag (по умолчанию: full)
# Требует: bash, make, awk, grep, printf, python3 (для генерации длинных строк)

set -Eeuo pipefail
shopt -s nocasematch   # [[ =~ ]] без учёта регистра

MODE="${1:-full}"   # fast|full|diag
ROOT="$(pwd)"

# ---------- util ----------
RED() { printf "\033[31m%s\033[0m\n" "$*"; }
GRN() { printf "\033[32m%s\033[0m\n" "$*"; }
YLW() { printf "\033[33m%s\033[0m\n" "$*"; }
BLU() { printf "\033[34m%s\033[0m\n" "$*"; }

pass(){ GRN "PASS: $*"; }
fail(){ RED "FAIL: $*"; exit 1; }
section(){ BLU "\n=== $* ==="; }

# run, capture rc/out/err
run() { # $@ -> rc,out,err to globals
  local __tmpd; __tmpd="$(mktemp -d)"
  set +e
  "$@" >"$__tmpd/out" 2>"$__tmpd/err"
  rc=$?
  set -e
  out="$(cat "$__tmpd/out")"
  err="$(cat "$__tmpd/err")"
  rm -rf "$__tmpd"
}

# assert helpers
# Варианты:
#   assert_rc <rc> -- <cmd ...>             # без сообщения
#   assert_rc <rc> <cmd ...> "<human msg>"  # с сообщением
assert_rc() {
  local exp_rc="$1"; shift
  local msg=""
  local -a cmd=()

  if [[ "$1" == "--" ]]; then
    shift
    cmd=( "$@" )              # режим без сообщения
  else
    local n=$(($#-1))
    cmd=( "${@:1:$n}" )
    msg="${@: -1}"
  fi

  set +e
  "${cmd[@]}" >/dev/null 2>&1
  local got=$?
  set -e

  if [[ $got -ne $exp_rc ]]; then
    echo "cmd: ${cmd[*]}"
    echo "rc expected=$exp_rc got=$got"
    [[ -n "$msg" ]] && fail "$msg" || fail "wrong rc"
  fi
}

assert_eq(){
  local exp="$1"; local got="$2"; local msg="$3"
  # нормализуем: CRLF -> LF, убираем хвостовые пробелы и пустые строки в конце
  local exp_n got_n
  exp_n="$(printf "%s" "$exp" | sed -e 's/\r$//' -e 's/[[:space:]]\+$//' )"
  got_n="$(printf "%s" "$got" | sed -e 's/\r$//' -e 's/[[:space:]]\+$//' )"
  if [[ "$got_n" != "$exp_n" ]]; then
    echo "expected:"; printf "%s\n" "$exp"
    echo "---got---"; printf "%s\n" "$got"
    fail "$msg"
  fi
}
assert_re(){ local rx="$1"; local got="$2"; local msg="$3"; [[ "$got" =~ $rx ]] || { echo "regex: $rx"; echo "---got---"; echo "$got"; fail "$msg"; } }

# diagnostics (не валится)
diag(){ # $desc ; cmd...
  local desc="$1"; shift
  echo -e "\n--- $desc ---"
  echo "CMD: $*"
  run "$@"
  echo "RC: $rc"
  [[ -n "$out" ]] && { echo "STDOUT:"; echo "$out"; }
  [[ -n "$err" ]] && { echo "STDERR:"; echo "$err"; }
  echo "----"
}

# подготовка
tmpdir="$(mktemp -d)"
cleanup(){ rm -rf "$tmpdir"; }
trap cleanup EXIT

# ---------- build ----------
YLW ">>> Building all..."
if ! make -s all >/dev/null; then
  fail "Build failed"
fi
pass "Build success"

# ===================================================================================
# TASK 1
# ===================================================================================
section "task1 (валидные/краевые/мусор)"

# валидные
run ./task1 100 -a;          assert_eq "5050" "$out" "task1 -a 100"
assert_rc 3 -- ./task1 "   ---10   " -a
run ./task1 29 -p;           assert_re "прост" "$out" "task1 -p prime"
run ./task1 100 -p;          assert_re "состав" "$out" "task1 -p composite"
run ./task1 -255 -s;         assert_eq "- F F" "$out" "task1 -s negative"
run ./task1 0 -s;            assert_eq "0"     "$out" "task1 -s zero"
run ./task1 10 -f;           assert_eq "3628800" "$out" "task1 -f 10!"

# пределы/ошибки
assert_rc 3 -- ./task1 0 -e
assert_rc 3 -- ./task1 11 -e
run ./task1 21 -f; [[ $rc -eq 5 && "$out" == "OVERFLOW" ]] || fail "task1 -f overflow"
run ./task1 30 /A;           assert_eq "465" "$out" "task1 flag '/A'"
assert_rc 2 -- ./task1 '123abc' -a
assert_rc 2 -- ./task1 '--' -a
assert_rc 1 -- ./task1 100
assert_rc 1 -- ./task1 100 -z
assert_rc 1 -- ./task1

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "spaces as number" ./task1 "   " -a
  diag "tab/newline as number" ./task1 $'\t\n' -a
  bignum="$(python3 - <<'PY'
print('1'*2000)
PY
)"
  diag "very long number" ./task1 "$bignum" -a
  diag "double sign '+--10'" ./task1 +--10 -a
  diag "cyrillic number" ./task1 абвгд -a
  diag "symbol flag" ./task1 10 -@
fi
pass "task1 OK"

# ===================================================================================
# TASK 2
# ===================================================================================
section "task2 (n-th prime)"

# базовые
exp=$'2\n29\n541'
run bash -lc "printf -- '3\n1\n10\n100\n' | ./task2";  assert_eq "$exp"   "$out" "task2 basic"
run bash -lc "printf -- '  2 \n  1 \n 10 \n' | ./task2"; assert_eq $'2\n29' "$out" "task2 spaces"

# ошибки
assert_rc 1 -- bash -lc "printf -- '0\n' | ./task2"
assert_rc 2 -- bash -lc "printf -- '2\n10\n' | ./task2"
assert_rc 1 -- bash -lc "printf -- '\n' | ./task2"
assert_rc 1 -- bash -lc "printf -- 'abc\n' | ./task2"
assert_rc 2 -- bash -lc "printf -- '2\nx\n10\n' | ./task2"

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "negative T" bash -lc "printf -- '-1\n' | ./task2"
  hugeT="$(python3 - <<'PY'
print('100000000000000000000000')
PY
)"
  diag "huge T" bash -lc "printf -- '$hugeT\n1\n' | ./task2"
fi
pass "task2 OK"

# ===================================================================================
# TASK 3
# ===================================================================================
section "task3 (-m, -t, -q)"

# -м
run ./task3 -m 12 4; assert_re "кратн" "$out" "task3 -m 12 4"
run ./task3 -m 7 3;  assert_re "не крат" "$out" "task3 -m 7 3"
assert_rc 3 -- ./task3 -m 10 0

# -t
run ./task3 -t 1e-9 3 4 5; assert_re "прямоуголь" "$out" "task3 -t 3 4 5"
run ./task3 -t 1e-9 1 2 3; assert_re "не (треугольник|прямоуголь)" "$out" "task3 -t 1 2 3"
assert_rc 3 -- ./task3 -t 0 3 4 5
assert_rc 3 -- ./task3 -t 1e-9 -1 2 2

# -q (валидный эпсилон и регистр)
run ./task3 /q 1e-9 1 0 -1; assert_re "нет решений|x=|x1=" "$out" "task3 -q"
# а вот кривой эпсилон должен дать ошибку парсинга
assert_rc 2 -- ./task3 -q ---1e-9 1 0 -1

# флаги/ошибки
run ./task3 -M 12 3; assert_re "кратн" "$out" "task3 -M flag"
assert_rc 1 -- ./task3 -x 1 2 3
assert_rc 1 -- ./task3
assert_rc 2 -- ./task3 -t 1e-9 3 4 a

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "empty args" ./task3 -m '' ''
  diag "cyrillic arg" ./task3 -m абв 10
  diag "huge int" ./task3 -m 100000000000000000000000 1
  diag "symbol flag newline" ./task3 $'-\n' 1 2
fi
pass "task3 OK"

# ===================================================================================
# TASK 4
# ===================================================================================
section "task4 (constants)"

# sqrt2/e: уравнения (узкие коридоры)
val="$(./task4 Sqrt2 EqUaTiOn 1e-12)"; awk -v x="$val" 'BEGIN{exit !(x>1.4142 && x<1.4143)}' || fail "task4 sqrt2 equation"
val="$(./task4 e equation 1e-10)";       awk -v x="$val" 'BEGIN{exit !(x>2.7182 && x<2.7184)}' || fail "task4 e equation"

# pi series (свободный коридор)
val="$(./task4 pi series 1e-6)";  awk -v x="$val" 'BEGIN{exit !(x>3.13 && x<3.16)}' || fail "task4 pi series"

# ln2 limit (свободный коридор)
val="$(./task4 ln2 limit 1e-6)"; awk -v x="$val" 'BEGIN{exit !(x>0.69 && x<0.70)}' || fail "task4 ln2 limit"

# ошибки
assert_rc 1 -- ./task4 eeee series 1e-6
assert_rc 2 -- ./task4 pi series abc
assert_rc 1 -- ./task4 pi bad 1e-6

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "empty const" ./task4 '' series 1e-6
  diag "empty method" ./task4 pi '' 1e-6
  diag "neg eps" ./task4 pi series -100
  hugeeps="$(python3 - <<'PY'
print('1'*24)
PY
)"
  diag "huge eps" ./task4 pi series "$hugeeps"
  diag "symbol method" ./task4 pi @@@@ 1e-6
fi
pass "task4 OK"

# ===================================================================================
# TASK 5
# ===================================================================================
section "task5 (files)"

echo -e "A1b2C3\nabv!? \n" > "$tmpdir/t5.txt"

# -ni, -nd, -ns, -na и out_*
run ./task5 -NI "$tmpdir/t5.txt" "$tmpdir/t5_i.out"; [[ -s "$tmpdir/t5_i.out" ]] || fail "-ni empty out"
run ./task5 -Nd "$tmpdir/t5.txt" "$tmpdir/t5_d.out"; grep -qv "[0-9]" "$tmpdir/t5_d.out" || fail "-nd digits remained"
run ./task5 -Ns "$tmpdir/t5.txt" "$tmpdir/t5_s.out"; [[ -s "$tmpdir/t5_s.out" ]] || fail "-ns empty out"
run ./task5 -na "$tmpdir/t5.txt" "$tmpdir/t5_a.out"; [[ -s "$tmpdir/t5_a.out" ]] || fail "-na empty out"
run ./task5 -a  "$tmpdir/t5.txt"; [[ -s "out_$(basename "$tmpdir/t5.txt")" ]] || fail "-a default out_ missing"

# ошибки
assert_rc 4 -- ./task5 -ni "$tmpdir/nope.txt" "$tmpdir/out.txt"
assert_rc 1 -- ./task5 -ni "$tmpdir/t5.txt"
assert_rc 1 -- ./task5 -z "$tmpdir/t5.txt"

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  : > "$tmpdir/t5_empty.txt"
  diag "empty file -ni" ./task5 -ni "$tmpdir/t5_empty.txt" "$tmpdir/t5_e.out"
  printf '!@#$%%^&*()\n' > "$tmpdir/t5_spec.txt"
  diag "spec file -ni" ./task5 -ni "$tmpdir/t5_spec.txt" "$tmpdir/t5_i_spec.out"
  longline="$(python3 - <<'PY'
print('A'*2000)
PY
)"; printf "%s\n" "$longline" > "$tmpdir/t5_long.txt"
  diag "long line -ni" ./task5 -ni "$tmpdir/t5_long.txt" "$tmpdir/t5_i_long.out"
fi
pass "task5 OK"

# ===================================================================================
# TASK 6
# ===================================================================================
section "task6 (integrals)"

val="$(./task6 1e-5 1 0 1)"; awk -v x="$val" 'BEGIN{exit !(x>0.746 && x<0.748)}' || fail "task6 f1"
val="$(./task6 1e-4 2 -10 10)"; awk -v x="$val" 'BEGIN{exit !(x>3.25 && x<3.36)}' || fail "task6 f2"

set +e; ./task6 1e-4 1 1 0 >/dev/null; got=$?; set -e; [[ $got -eq 0 ]] || fail "task6 reversed limits"
assert_rc 1 -- ./task6 0 1 0 1
assert_rc 1 -- ./task6 1e-4 99 0 1
assert_rc 2 -- ./task6 1e-4 1 a b

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "empty eps" ./task6 '' 1 0 1
  diag "symbol eps" ./task6 @@@ 1 0 1
fi
pass "task6 OK"

# ===================================================================================
# TASK 7
# ===================================================================================
section "task7 (bases 2..36, minimal base, OVERFLOW/INVALID)"

cat > "$tmpdir/t7.in" <<'EOF'
000F
+0000
-00000123
Z
10ABC
--7
9223372036854775808
0
-0
+0
A
a
EOF

run ./task7 "$tmpdir/t7.in" "$tmpdir/t7.out"; [[ -s "$tmpdir/t7.out" ]] || fail "task7 empty out"

grep -E "^F 16 15$"        "$tmpdir/t7.out" >/dev/null || fail "F base16"
grep -E "^0 [0-9]+ 0$"     "$tmpdir/t7.out" >/dev/null || pass "+0000 as 0"
grep -E "^-?123 [0-9]+ -?123$" "$tmpdir/t7.out" >/dev/null || pass "-00000123"
grep -E "^Z 36 35$"        "$tmpdir/t7.out" >/dev/null || fail "Z base36"
grep -E "^7 [0-9]+ 7$"     "$tmpdir/t7.out" >/dev/null || pass "--7 collapse"
grep -E "^9223372036854775808 10 OVERFLOW$" "$tmpdir/t7.out" >/dev/null || fail "overflow 2^63"
grep -E "^0 [0-9]+ 0$"     "$tmpdir/t7.out" >/dev/null || fail "zero"
grep -E "^A 11 10$"        "$tmpdir/t7.out" >/dev/null || fail "A base11"

# ошибки
assert_rc 4 -- ./task7 "$tmpdir/nope.in" "$tmpdir/out.txt"
assert_rc 1 -- ./task7 "$tmpdir/t7.in"

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  : > "$tmpdir/t7_empty.txt"
  diag "empty file" ./task7 "$tmpdir/t7_empty.txt" "$tmpdir/t7_e.out"
  longline="$(python3 - <<'PY'
print('F'*2000)
PY
)"; printf "%s\n" "$longline" > "$tmpdir/t7_long.txt"
  diag "long line" ./task7 "$tmpdir/t7_long.txt" "$tmpdir/t7_out_long.txt"
fi
pass "task7 OK"

# ===================================================================================
# TASK 8
# ===================================================================================
section "task8 (max by abs, Stop, conversions)"

exp=$'F\n15\n15\n15\n15'
run bash -lc "printf -- '16\n-000000F\n+7\nStop\n' | ./task8";  assert_eq "$exp" "$out" "base16 example"

assert_rc 0 -- bash -lc "printf -- '10\n1\nSTOP\n' | ./task8"
assert_rc 1 -- bash -lc "printf -- '1\nStop\n'  | ./task8"
assert_rc 1 -- bash -lc "printf -- '37\nStop\n' | ./task8"
assert_rc 1 -- bash -lc "printf -- '\nStop\n' | ./task8"

exp=$'100\n4\n4\n4\n4\n'
run bash -lc "printf -- '2\n-100\n+11\nStop\n' | ./task8";  assert_eq "$exp" "$out" "pick max by abs"

exp=$'9223372036854775808\nOVERFLOW\nOVERFLOW\nOVERFLOW\nOVERFLOW'
run bash -lc "printf -- '10\n9223372036854775808\nStop\n' | ./task8";  assert_eq "$exp" "$out" "overflow to 4x OVERFLOW"

run bash -lc "printf -- '  16 \n   ---F \n   ++++++F \n  sToP \n' | ./task8";  assert_re "^F" "$out" "mixed signs + stop"
run bash -lc "printf -- '10\nStop\n' | ./task8";                               [[ $rc -eq 0 ]] || fail "no numbers must not crash"

# супер-абсурд (diag)
if [[ "$MODE" == "diag" ]]; then
  diag "symbol base" bash -lc "printf -- '@\nStop\n' | ./task8"
  hugebase="$(python3 - <<'PY'
print('1'*24)
PY
)"
  diag "huge base" bash -lc "printf -- '$hugebase\nStop\n' | ./task8"
  diag "symbol number" bash -lc "printf -- '10\n@\nStop\n' | ./task8"
  hugenum="$(python3 - <<'PY'
print('-' + '1'*2000)
PY
)"
  diag "huge number" bash -lc "printf -- '10\n$hugenum\nStop\n' | ./task8"
fi
pass "task8 OK"

GRN "ALL TESTS PASSED (mode: $MODE)."
