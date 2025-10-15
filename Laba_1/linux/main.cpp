#include <iostream>
#include <iomanip>
#include <cmath>
#include "number.h"
#include "vector.h"

void testNumberAdvancedOperations() {
    std::cout << "============= ТЕСТИРОВАНИЕ NUMBER =============\n";
    
    Number pi = Number::create(3.14159);
    Number e = Number::create(2.71828);
    Number sqrt2 = Number::create(1.41421);
    
    std::cout << "\nМматематические константы:\n";
    std::cout << "π = " << pi.getValue() << ", e = " << e.getValue() << ", √2 = " << sqrt2.getValue() << "\n";
    
    std::cout << "\nСложные операции:\n";
    std::cout << "π * e = " << (pi * e).getValue() << " (≈8.5397)\n";
    std::cout << "e^π = " << (e ^ pi).getValue() << " (≈23.1407)\n";
    std::cout << "π^e = " << (pi ^ e).getValue() << " (≈22.4592)\n";
    
    std::cout << "\nОператоры присваивания:\n";
    Number x = Number::create(10.0);
    Number y = Number::create(3.0);
    
    std::cout << "Исходное: x = " << x.getValue() << ", y = " << y.getValue() << "\n";
    x += y;
    std::cout << "После x += y: x = " << x.getValue() << " (ожидается: 13)\n";
    x -= Number::create(5.0);
    std::cout << "После x -= 5: x = " << x.getValue() << " (ожидается: 8)\n";
    x *= Number::create(2.0);
    std::cout << "После x *= 2: x = " << x.getValue() << " (ожидается: 16)\n";
    x /= Number::create(4.0);
    std::cout << "После x /= 4: x = " << x.getValue() << " (ожидается: 4)\n";
    
    std::cout << "\nОператоры сравнения:\n";
    Number a = Number::create(7.5);
    Number b = Number::create(3.2);
    
    std::cout << a.getValue() << " == " << b.getValue() << ": " << (a == b) << " (false)\n";
    std::cout << a.getValue() << " != " << b.getValue() << ": " << (a != b) << " (true)\n";
    std::cout << a.getValue() << " > " << b.getValue() << ": " << (a > b) << " (true)\n";
    std::cout << a.getValue() << " < " << b.getValue() << ": " << (a < b) << " (false)\n";
    std::cout << a.getValue() << " >= " << b.getValue() << ": " << (a >= b) << " (true)\n";
    std::cout << a.getValue() << " <= " << b.getValue() << ": " << (a <= b) << " (false)\n";
    
    std::cout << "\nОстаток от деления:\n";
    std::cout << "7.5 % 3.2 = " << (a % b).getValue() << " (ожидается: 1.1)\n";
    std::cout << "15.7 % 4.2 = " << (Number::create(15.7) % Number::create(4.2)).getValue() << " (≈3.1)\n";
    
    std::cout << "\nГраничные случаи:\n";
    try {
        Number zero = Number::ZERO;
        Number result = a % zero;
        std::cout << "Ошибка: исключение при делении на ноль не сгенерировано\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Все норм: " << e.what() << "\n";
    }
    
    std::cout << "\nКонстанты:\n";
    std::cout << "ONE + ONE = " << (Number::ONE + Number::ONE).getValue() << " (2)\n";
    std::cout << "ONE ^ 10 = " << (Number::ONE ^ Number::create(10.0)).getValue() << " (1)\n";
    std::cout << "ZERO == ZERO: " << (Number::ZERO == Number::ZERO) << " (true)\n";
}

void testVectorAdvancedOperations() {
    std::cout << "\n\n============= РАСШИРЕННОЕ ТЕСТИРОВАНИЕ КЛАССА VECTOR =============\n";
    
    Vector unitCircle45(Number::create(0.7071), Number::create(0.7071));
    Vector unitCircle30(Number::create(0.8660), Number::create(0.5));
    Vector vecA(Number::create(3.0), Number::create(4.0));
    Vector vecB(Number::create(-2.0), Number::create(5.0));
    Vector vecC(Number::create(-3.0), Number::create(-4.0));
    
    std::cout << "\nИнтересные векторы:\n";
    std::cout << "vecA (3,4) - египетский треугольник\n";
    std::cout << "vecB (-2,5) - второй квадрант\n";
    std::cout << "vecC (-3,-4) - третий квадрант\n";
    std::cout << "unit45 (0.7071,0.7071) - единичный под 45°\n";
    std::cout << "unit30 (0.8660,0.5) - единичный под 30°\n";
    
    std::cout << "\nПолярные координаты (модуль и угол):\n";
    std::cout << std::fixed << std::setprecision(4);
    
    Number rhoA = vecA.getRho();
    Number thetaA = vecA.getTheta();
    std::cout << "vecA: ρ = " << rhoA.getValue() << " (ожидается: 5.0000), ";
    std::cout << "θ = " << thetaA.getValue() << " рад (" << thetaA.getValue() * 180 / M_PI << "°)\n";
    
    Number rhoB = vecB.getRho();
    Number thetaB = vecB.getTheta();
    std::cout << "vecB: ρ = " << rhoB.getValue() << " (≈5.3852), ";
    std::cout << "θ = " << thetaB.getValue() << " рад (" << thetaB.getValue() * 180 / M_PI << "°) - второй квадрант\n";
    
    Number rhoUnit45 = unitCircle45.getRho();
    Number thetaUnit45 = unitCircle45.getTheta();
    std::cout << "unit45: ρ = " << rhoUnit45.getValue() << " (≈1.0000), ";
    std::cout << "θ = " << thetaUnit45.getValue() << " рад (" << thetaUnit45.getValue() * 180 / M_PI << "°) - должно быть 45°\n";
    
    std::cout << "\nСложение векторов:\n";
    Vector sumAB = vecA + vecB;
    std::cout << "vecA + vecB = (" << sumAB.getX().getValue() << ", " << sumAB.getY().getValue() << ") (ожидается: (1, 9))\n";
    
    Vector sumChain = vecA + vecB + vecC;
    std::cout << "vecA + vecB + vecC = (" << sumChain.getX().getValue() << ", " << sumChain.getY().getValue() << ") (ожидается: (-2, 5))\n";
    
    std::cout << "\nЦепочка операций:\n";
    Vector complexOp = unitCircle45 + unitCircle30;
    std::cout << "unit45 + unit30 = (" << complexOp.getX().getValue() << ", " << complexOp.getY().getValue() << ") (≈1.5731, 1.2071)\n";
    std::cout << "Модуль суммы: " << complexOp.getRho().getValue() << " (≈1.9829)\n";

    std::cout << "\nТЕСТ НОВЫХ ФИЧ ВЕКТОРА:\n";

    Vector v3(Number::create(2.0), Number::create(-3.0));
    std::cout << "Вектор v3: (" << v3.getX().getValue() << ", " << v3.getY().getValue() << ")\n";
    std::cout << "Унарный минус для X: " << (-v3.getX()).getValue() << " (ожидается: -2)\n";
    std::cout << "Унарный минус для Y: " << (-v3.getY()).getValue() << " (ожидается: 3)\n";

    Vector base(Number::create(4.0), Number::create(0.0));
    Vector toProject(Number::create(3.0), Number::create(3.0));
    Vector projection = toProject.projectOnto(base);
    std::cout << "Проекция (3,3) на (4,0): (" << projection.getX().getValue() << ", " << projection.getY().getValue() << ") (ожидается: (3, 0))\n";

    Vector original(Number::create(2.0), Number::create(3.0));
    Vector perp = original.perpendicular();
    std::cout << "Перпендикуляр к (2,3): (" << perp.getX().getValue() << ", " << perp.getY().getValue() << ") (ожидается: (-3, 2))\n";
    std::cout << "Проверка ортогональности: " << original.dot(perp).getValue() << " (ожидается: 0)\n";

    Vector vec(Number::create(1.0), Number::create(0.0));
    Vector rotated90 = vec.rotate(Number::create(M_PI / 2));
    std::cout << "Поворот (1,0) на 90°: (" << rotated90.getX().getValue() << ", " << rotated90.getY().getValue() << ") (ожидается: (0, 1))\n";

    Vector polar = Vector::fromPolar(Number::create(2.0), Number::create(M_PI / 3));
    std::cout << "Из полярных (ρ=2, θ=60°): (" << polar.getX().getValue() << ", " << polar.getY().getValue() << ") (ожидается: (1, 1.732))\n";

    Vector v4(Number::create(1.0), Number::create(1.0));
    Vector v5(Number::create(2.0), Number::create(2.0));
    v4 += v5;
    std::cout << "После v4 += v5: (" << v4.getX().getValue() << ", " << v4.getY().getValue() << ") (ожидается: (3, 3))\n";
    v4 *= Number::create(2.0);
    std::cout << "После v4 *= 2: (" << v4.getX().getValue() << ", " << v4.getY().getValue() << ") (ожидается: (6, 6))\n";
}

void testPhysicalScenarios() {
    std::cout << "\n\n============= ФИЗИЧЕСКИЕ ПРИКОЛЫ =============\n";
    
    std::cout << "\nПрикол 1: Сложение сил\n";
    Vector force1(Number::create(10.0), Number::create(0.0));
    Vector force2(Number::create(0.0), Number::create(15.0));
    Vector totalForce = force1 + force2;
    std::cout << "F_1 (10,0) + F_2 (0,15) = (" << totalForce.getX().getValue() << ", " << totalForce.getY().getValue() << ")\n";
    std::cout << "Результирующая сила: " << totalForce.getRho().getValue() << " Н под углом " 
              << totalForce.getTheta().getValue() * 180 / M_PI << "°\n";
    
    std::cout << "\nПрикол 2: Навигация\n";
    Vector displacement1(Number::create(5.0), Number::create(12.0));
    Vector displacement2(Number::create(8.0), Number::create(-6.0));
    Vector totalDisplacement = displacement1 + displacement2;
    std::cout << "S_1 (5,12) + S_2 (8,-6) = (" << totalDisplacement.getX().getValue() << ", " << totalDisplacement.getY().getValue() << ")\n";
    std::cout << "Общее перемещение: " << totalDisplacement.getRho().getValue() << " единиц\n";
    std::cout << "Направление: " << totalDisplacement.getTheta().getValue() * 180 / M_PI << "° от севера\n";
    
    std::cout << "\nПрикол 3: Электрическое поле\n";
    Vector fieldA(Number::create(3.0), Number::create(4.0));
    Vector fieldB(Number::create(1.0), Number::create(1.0));
    Vector totalField = fieldA + fieldB;
    std::cout << "F_A (3,4) + F_B (1,1) = (" << totalField.getX().getValue() << ", " << totalField.getY().getValue() << ")\n";
    std::cout << "Напряженность суммарного поля: " << totalField.getRho().getValue() << " В/м\n";
}

void testEdgeAndSpecialCases() {
    std::cout << "\n\n============= ГРАНИЧНЫЕ И ОСОБЫЕ СЛУЧАИ =============\n";
    
    std::cout << "\nОсобые векторы:\n";
    std::cout << "Vector::ZERO = (" << Vector::ZERO.getX().getValue() << ", " << Vector::ZERO.getY().getValue() << ")\n";
    std::cout << "Vector::ONE = (" << Vector::ONE.getX().getValue() << ", " << Vector::ONE.getY().getValue() << ")\n";
    
    std::cout << "\nНулевой вектор:\n";
    Vector zero = Vector::ZERO;
    std::cout << "Модуль нулевого вектора: " << zero.getRho().getValue() << " (0)\n";
    std::cout << "Угол нулевого вектора: " << zero.getTheta().getValue() << " радиан (0)\n";
    
    std::cout << "\nКопирование и присваивание:\n";
    Vector original(Number::create(7.0), Number::create(24.0));
    Vector copy = original;
    Vector assigned;
    assigned = original;
    
    std::cout << "Оригинал: (" << original.getX().getValue() << ", " << original.getY().getValue() << ")\n";
    std::cout << "Копия: (" << copy.getX().getValue() << ", " << copy.getY().getValue() << ")\n";
    std::cout << "Присвоенный: (" << assigned.getX().getValue() << ", " << assigned.getY().getValue() << ")\n";
    std::cout << "Модуль всех: " << original.getRho().getValue() << " (25)\n";
}

int main() {
    std::cout << std::boolalpha;
    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "ЗАПУСК ТЕСТОВ NUMBER И VECTOR\n";
    std::cout << "========================================================================\n";
    
    testNumberAdvancedOperations();
    testVectorAdvancedOperations();
    testPhysicalScenarios();
    testEdgeAndSpecialCases();
    
    std::cout << "\nВсе тесты прошли\n";
    
    return 0;
}