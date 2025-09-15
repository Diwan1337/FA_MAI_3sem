#!/usr/bin/env bash
set -e

pass() { echo "✅ $1"; }
fail() { echo "❌ $1"; exit 1; }

# Соберём всё, если не собрано
make -s all >/dev/null || true

# task1
out=$(./task1 100 -a);  [ "$out" = "5050" ] && pass "task1 -a (sum)" || fail "task1 -a"
out=$(./task1 29 -p);   echo "$out" | grep -qi "простое" && pass "task1 -p (prime)" || fail "task1 -p"
./task1 -255 -s | grep -q "^- F F$" && pass "task1 -s (hex split)" || fail "task1 -s"

# task2
exp=$'2\n29\n541'
got=$(printf "3\n1\n10\n100\n" | ./task2)
[ "$got" = "$exp" ] && pass "task2 n-th primes" || { echo "got: $got"; fail "task2"; }

# task3
./task3 -m 12 4 | grep -q "кратно" && pass "task3 -m" || fail "task3 -m"
./task3 -t 1e-9 3 4 5 | grep -q "Прямоугольный" && pass "task3 -t" || fail "task3 -t"
./task3 -q 1e-9 1 0 -1 >/dev/null && pass "task3 -q" || fail "task3 -q"

# task4 (пороговые проверки по диапазону)
v=$(./task4 sqrt2 equation 1e-12); awk -v x="$v" 'BEGIN{exit !(x>1.4142 && x<1.4143)}' && pass "task4 sqrt2" || fail "task4 sqrt2"
v=$(./task4 e equation 1e-10);     awk -v x="$v" 'BEGIN{exit !(x>2.7182 && x<2.7184)}' && pass "task4 e" || fail "task4 e"

# task5
echo -e "A1b2C3\nабв!? \n" > t5.txt
# -i: посчитать латиницу по строкам
./task5 -ni t5.txt t5_i.out
# -d: удалить цифры
./task5 -nd t5.txt t5_d.out
# -s: посчитать "не [латиница|цифра|пробел]" по строкам
./task5 -ns t5.txt t5_s.out
# проверка, что файлы непустые
[ -s t5_i.out ] && [ -s t5_d.out ] && [ -s t5_s.out ] && pass "task5" || fail "task5"

# task6: ∫_0^1 e^{-x^2} dx ≈ 0.746824...
v=$(./task6 1e-4 1 0 1); awk -v x="$v" 'BEGIN{exit !(x>0.746 && x<0.748)}' && pass "task6 f1" || fail "task6 f1"

# task7: создаёт выходной файл
printf "000F\n1011\nZ\n" > t7.txt
./task7 t7.txt out7.txt
[ -s out7.txt ] && pass "task7" || fail "task7"

# task8
printf "16\n-000000F\n+7\nStop\n" | ./task8 > t8.out
grep -q "^F$" t8.out && pass "task8" || fail "task8"

echo "🎉 Все тесты прошли."
SH
chmod +x run_tests.sh
