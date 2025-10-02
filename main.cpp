#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double calculateDamage(double punchSpeed, double punchStrength,
                      double x_you, double y_you,
                      double x_opp, double y_opp,
                      bool isGuarding) {

    // Validate input parameters
    if (punchSpeed < 0 || punchSpeed > 100 ||
        punchStrength < 0 || punchStrength > 100 ||
        x_you < 0 || x_you > 20 ||
        y_you < 0 || y_you > 20 ||
        x_opp < 0 || x_opp > 20 ||
        y_opp < 0 || y_opp > 20) {
        return -999; // Invalid input
    }

    // Calculate distance between you and opponent using Cartesian distance formula
    double distance = sqrt(pow(x_you - x_opp, 2) + pow(y_you - y_opp, 2));

    // If distance is 0, input is invalid
    if (distance == 0) {
        return -999;
    }

    // Calculate base damage based on whether opponent is guarding
    double baseDamage;
    if (isGuarding) {
        // When guarding, strong punches are more effective
        baseDamage = 0.3 * punchSpeed + 0.7 * punchStrength;
    } else {
        // When not guarding, fast punches are more effective
        baseDamage = 0.7 * punchSpeed + 0.3 * punchStrength;
    }

    // Calculate distance multiplier based on distance ranges
    double distanceMultiplier;
    if (distance <= 10) {
        // Close range: double damage
        distanceMultiplier = 2.0;
    } else if (distance <= 15) {
        // Medium range: normal damage
        distanceMultiplier = 1.0;
    } else {
        // Far range: no damage
        distanceMultiplier = 0.0;
    }

    // Calculate effectiveness multiplier based on punch characteristics
    double effectivenessMultiplier;
    if (punchSpeed >= 90 || punchStrength >= 90) {
        // Too fast or too strong: no damage
        effectivenessMultiplier = 0.0;
    } else if (punchSpeed + punchStrength >= 150) {
        // Both fairly fast and strong: half damage
        effectivenessMultiplier = 0.5;
    } else if (punchSpeed <= 10 || punchStrength <= 10) {
        // Too slow or too weak: 80% damage reduction
        effectivenessMultiplier = 0.2;
    } else {
        // Normal conditions: full effectiveness
        effectivenessMultiplier = 1.0;
    }

    // Calculate final damage
    double damageDealt = baseDamage * distanceMultiplier * effectivenessMultiplier;

    return damageDealt;
}

struct TestCase {
    int id;
    double punchSpeed, punchStrength;
    double x_you, y_you, x_opp, y_opp;
    bool isGuarding;
    double expected;
};

int main() {
    vector<TestCase> testsBoundary = {
        {1, 0, 50, 12, 12, 8, 8, true, 14},
        {2, 1, 50, 12, 12, 8, 8, true, 14.12},
        {3, 99, 50, 12, 12, 8, 8, true, 0},
        {4, 100, 50, 12, 12, 8, 8, true, 0},
        {5, 50, 0, 12, 12, 8, 8, true, 6},
        {6, 50, 1, 12, 12, 8, 8, true, 6.28},
        {7, 50, 99, 12, 12, 8, 8, true, 0},
        {8, 50, 100, 12, 12, 8, 8, true, 0},
        {9, 50, 0, 12, 12, 8, 8, true, 6},
        {10, 50, 50, 0, 12, 8, 8, true, 100},
        {11, 50, 50, 19, 12, 8, 8, true, 50},
        {12, 50, 50, 20, 12, 8, 8, true, 50},
        {13, 50, 50, 12, 0, 8, 8, true, 100},
        {14, 50, 50, 12, 1, 8, 8, true, 100},
        {15, 50, 50, 12, 19, 8, 8, true, 50},
        {16, 50, 50, 12, 20, 8, 8, true, 0},
        {17, 50, 50, 12, 12, 0, 8, true, 50},
        {18, 50, 50, 12, 12, 1, 8, true, 100},
        {19, 50, 50, 12, 12, 19, 8, true, 100},
        {20, 50, 50, 12, 12, 20, 8, true, 100},
        {21, 50, 50, 12, 12, 8, 0, true, 50},
        {22, 50, 50, 12, 12, 8, 1, true, 50},
        {23, 50, 50, 12, 12, 8, 19, true, 100},
        {24, 50, 50, 12, 12, 8, 20, true, 100},
        {25, 50, 50, 12, 12, 8, 8, true, 100},
        {26, 50, 50, 12, 12, 8, 8, false, 100}
    };

    for (const auto &t : testsBoundary) {
        double result = calculateDamage(t.punchSpeed, t.punchStrength,
                                        t.x_you, t.y_you, t.x_opp, t.y_opp,
                                        t.isGuarding);
        cout << "Test " << t.id
             << ": expected=" << t.expected
             << ", got=" << result;
        if (fabs(result - t.expected) < 1e-2) cout << " [PASS]\n";
        else cout << " [FAIL]\n";
    }

    vector<TestCase> testDecisionTable = {
        {1, -10, 50, 5, 5, 10, 10, false, -999},
        {2, 110, 50, 5, 5, 10, 10, false, -999},
        {3, 50, -10, 5, 5, 10, 10, false, -999},
        {4, 50, 110, 5, 5, 10, 10, false, -999},
        {5, 50, 50, -5, 5, 10, 10, false, -999},
        {6, 50, 50, 5, 5, -5, 10, false, -999},
        {7, 50, 50, 5, -5, 10, 10, false, -999},
        {8, 50, 50, 5, 5, 10, -5, false, -999},
        {9, 50, 50, 10, 10, 10, 10, false, -999},

        {10, 95, 50, 0, 0, 5, 0, false, 0},
        {11, 80, 80, 0, 0, 5, 0, false, 80},
        {12, 20, 5, 0, 0, 5, 0, false, 6.2},
        {13, 50, 50, 0, 0, 5, 0, false, 100},
        {14, 95, 50, 0, 0, 5, 0, true, 0},
        {15, 80, 80, 0, 0, 5, 0, true, 80},
        {16, 20, 5, 0, 0, 5, 0, true, 3.8},
        {17, 50, 50, 0, 0, 5, 0, true, 100},

        {18, 95, 50, 0, 0, 12, 0, false, 0},
        {19, 80, 80, 0, 0, 12, 0, false, 40},
        {20, 20, 5, 0, 0, 12, 0, false, 3.1},
        {21, 50, 50, 0, 0, 12, 0, false, 50},
        {22, 95, 50, 0, 0, 12, 0, true, 0},
        {23, 80, 80, 0, 0, 12, 0, true, 40},
        {24, 20, 5, 0, 0, 12, 0, true, 1.9},
        {25, 50, 50, 0, 0, 12, 0, true, 50},

        {26, 95, 50, 0, 0, 18, 0, false, 0},
        {27, 80, 80, 0, 0, 18, 0, false, 0},
        {28, 20, 5, 0, 0, 18, 0, false, 0},
        {29, 50, 50, 0, 0, 18, 0, false, 0},
        {30, 95, 50, 0, 0, 18, 0, true, 0},
        {31, 80, 80, 0, 0, 18, 0, true, 0},
        {32, 20, 5, 0, 0, 18, 0, true, 0},
        {33, 50, 50, 0, 0, 18, 0, true, 0}
    };


    for (const auto &t : testDecisionTable) {
        double result = calculateDamage(t.punchSpeed, t.punchStrength,
                                        t.x_you, t.y_you, t.x_opp, t.y_opp,
                                        t.isGuarding);
        cout << "Test " << t.id
             << ": expected=" << t.expected
             << ", got=" << result;
        if (fabs(result - t.expected) < 1e-2) cout << " [PASS]\n";
        else cout << " [FAIL]\n";
    }

    return 0;
}