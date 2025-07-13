#include <vector>
#include "obstacle.h"
#include "raylib.h"

struct IsoObstacle {
    Vector2 topLeft;
    Vector2 topRight;
    Vector2 bottomRight;
    Vector2 bottomLeft;
};

std::vector<IsoObstacle> isoObstacles;

bool pointInQuad(Vector2 p, const IsoObstacle& q) {
    int count = 0;
    Vector2 corners[4] = { q.topLeft, q.topRight, q.bottomRight, q.bottomLeft };

    for (int i = 0; i < 4; ++i) {
        Vector2 a = corners[i];
        Vector2 b = corners[(i + 1) % 4];
        if (((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y + 0.0001f) + a.x)) {
            count++;
        }
    }
    return (count % 2 == 1);
}

void add_isometric_obstacle(Vector2 topLeft, Vector2 topRight, Vector2 bottomRight, Vector2 bottomLeft) {
    IsoObstacle quad = { topLeft, topRight, bottomRight, bottomLeft };
    isoObstacles.push_back(quad);
}

bool is_colliding(float x,float y) {
    float radius = 20.0f;
    for (const auto& quad : isoObstacles) {
        for (float angle = 0; angle < 2 * PI; angle += PI / 8) {
            Vector2 offset = {
                x + radius * cosf(angle),
                y + radius * sinf(angle)
            };
            if (pointInQuad(offset, quad)) return true;
        }
    }
    return false;
}

void LoadIsometricObstacles() {
    add_isometric_obstacle(
    (Vector2){4280, 2814},
    (Vector2){4328, 2836},
    (Vector2){4327, 3015},
    (Vector2){4280, 2997}
);

add_isometric_obstacle(
    (Vector2){4329, 2839},
    (Vector2){4360, 2839},
    (Vector2){4362, 3017},
    (Vector2){4329, 3014}
);

add_isometric_obstacle(
    (Vector2){4364, 2838},
    (Vector2){4407, 2798},
    (Vector2){4412, 2990},
    (Vector2){4361, 3018}
);

add_isometric_obstacle(
    (Vector2){4105, 2726},
    (Vector2){4135, 2726},
    (Vector2){4134, 2903},
    (Vector2){4106, 2904}
);

add_isometric_obstacle(
    (Vector2){4102, 2903},
    (Vector2){4058, 2885},
    (Vector2){4054, 2690},
    (Vector2){4104, 2719}
);

add_isometric_obstacle(
    (Vector2){4057, 2686},
    (Vector2){4107, 2659},
    (Vector2){4186, 2699},
    (Vector2){4134, 2723}
);

add_isometric_obstacle(
    (Vector2){4134, 2725},
    (Vector2){4185, 2697},
    (Vector2){4188, 2883},
    (Vector2){4134, 2905}
);

add_isometric_obstacle(
    (Vector2){4187, 2883},
    (Vector2){4185, 2852},
    (Vector2){4231, 2844},
    (Vector2){4231, 2873}
);

add_isometric_obstacle(
    (Vector2){4113, 2659},
    (Vector2){4141, 2660},
    (Vector2){4185, 2684},
    (Vector2){4183, 2700}
);

add_isometric_obstacle(
    (Vector2){4279, 2806},
    (Vector2){4331, 2774},
    (Vector2){4389, 2804},
    (Vector2){4360, 2835}
);

add_isometric_obstacle(
    (Vector2){4338, 2771},
    (Vector2){4361, 2774},
    (Vector2){4401, 2794},
    (Vector2){4391, 2804}
);

add_isometric_obstacle(
    (Vector2){4311, 2542},
    (Vector2){4312, 2718},
    (Vector2){4390, 2685},
    (Vector2){4391, 2503}
);

add_isometric_obstacle(
    (Vector2){4310, 2540},
    (Vector2){4232, 2500},
    (Vector2){4233, 2682},
    (Vector2){4308, 2716}
);

add_isometric_obstacle(
    (Vector2){4233, 2502},
    (Vector2){4312, 2543},
    (Vector2){4390, 2500},
    (Vector2){4312, 2460}
);

add_isometric_obstacle(
    (Vector2){4424, 2408},
    (Vector2){4506, 2447},
    (Vector2){4507, 2627},
    (Vector2){4427, 2590}
);

add_isometric_obstacle(
    (Vector2){4505, 2447},
    (Vector2){4581, 2408},
    (Vector2){4586, 2589},
    (Vector2){4509, 2627}
);

add_isometric_obstacle(
    (Vector2){4425, 2407},
    (Vector2){4503, 2371},
    (Vector2){4579, 2408},
    (Vector2){4504, 2447}
);

add_isometric_obstacle(
    (Vector2){4391.14, 2606.38},
    (Vector2){4441.14, 2589.38},
    (Vector2){4440.14, 2612.38},
    (Vector2){4393.14, 2628.38}
);

add_isometric_obstacle(
    (Vector2){4447.14, 2794.38},
    (Vector2){4447.14, 2763.38},
    (Vector2){4519.14, 2777.38},
    (Vector2){4519.14, 2810.38}
);

add_isometric_obstacle(
    (Vector2){4618.14, 2542.38},
    (Vector2){4619.14, 2572.38},
    (Vector2){4678.14, 2555.38},
    (Vector2){4678.14, 2525.38}
);

add_isometric_obstacle(
    (Vector2){4966.95, 2832.19},
    (Vector2){4618.95, 2655.19},
    (Vector2){4716.95, 2584.19},
    (Vector2){5060.95, 2773.19}
);

add_isometric_obstacle(
    (Vector2){4747.43, 2494.15},
    (Vector2){4779.76, 2490.56},
    (Vector2){4775.76, 2510.56},
    (Vector2){4748.76, 2512.56}
);

add_isometric_obstacle(
    (Vector2){4753.1, 2981.22},
    (Vector2){4751.1, 3009.22},
    (Vector2){4189.1, 2705.22},
    (Vector2){4184.1, 2727.22}
);

add_isometric_obstacle(
    (Vector2){4184.1, 2727.22},
    (Vector2){4184.1, 2700.22},
    (Vector2){4753.1, 2982.22},
    (Vector2){4751.1, 3005.22}
);

add_isometric_obstacle(
    (Vector2){4048.51, 2693.78},
    (Vector2){3917.51, 2759.78},
    (Vector2){3923.51, 2778.78},
    (Vector2){4048.51, 2720.78}
);

add_isometric_obstacle(
    (Vector2){3858.51, 2796.78},
    (Vector2){3855.51, 2819.78},
    (Vector2){4491.51, 3139.78},
    (Vector2){4496.51, 3115.78}
);

add_isometric_obstacle(
    (Vector2){3659.43, 2439},
    (Vector2){3878.43, 2551},
    (Vector2){4038.43, 2471},
    (Vector2){3818.43, 2358}
);

add_isometric_obstacle(
    (Vector2){3878.43, 2547},
    (Vector2){3880.43, 2579},
    (Vector2){4041.43, 2499},
    (Vector2){4039.43, 2469}
);

add_isometric_obstacle(
    (Vector2){3874.43, 2548},
    (Vector2){3880.43, 2578},
    (Vector2){3654.43, 2467},
    (Vector2){3656.43, 2437}
);

add_isometric_obstacle(
    (Vector2){4269.43, 2297},
    (Vector2){4375.43, 2313},
    (Vector2){4372.43, 2360},
    (Vector2){4279.43, 2345}
);

add_isometric_obstacle(
    (Vector2){3882.43, 2239},
    (Vector2){3889.95, 2264.62},
    (Vector2){4042.95, 2183.62},
    (Vector2){4042.95, 2159.62}
);

add_isometric_obstacle(
    (Vector2){4046.88, 2157.1},
    (Vector2){4435.88, 2344.1},
    (Vector2){4429.88, 2374.1},
    (Vector2){4046.88, 2182.1}
);

add_isometric_obstacle(
    (Vector2){3388.88, 2491.1},
    (Vector2){3402.88, 2499.1},
    (Vector2){3590.88, 2389.1},
    (Vector2){3590.88, 2408.1}
);

add_isometric_obstacle(
    (Vector2){3590.88, 2408.1},
    (Vector2){3590.88, 2408.1},
    (Vector2){3589.88, 2391.1},
    (Vector2){3384.88, 2492.1}
);

add_isometric_obstacle(
    (Vector2){3384.88, 2492.1},
    (Vector2){3403.88, 2501.1},
    (Vector2){3592.88, 2410.1},
    (Vector2){3592.88, 2390.1}
);

add_isometric_obstacle(
    (Vector2){3378.19, 2492.26},
    (Vector2){3917.19, 2753.26},
    (Vector2){3921.19, 2779.26},
    (Vector2){3378.19, 2516.26}
);

add_isometric_obstacle(
    (Vector2){3279.96, 2286.33},
    (Vector2){3674.96, 2088.33},
    (Vector2){3778.96, 2175.33},
    (Vector2){3415.96, 2359.33}
);

add_isometric_obstacle(
    (Vector2){3648.69, 1832.92},
    (Vector2){3658.69, 1688.92},
    (Vector2){3824.69, 1751.92},
    (Vector2){3811.69, 1909.92}
);

add_isometric_obstacle(
    (Vector2){3397.35, 1801.45},
    (Vector2){3685.35, 1944.45},
    (Vector2){3657.35, 2016.45},
    (Vector2){3368.35, 1878.45}
);

add_isometric_obstacle(
    (Vector2){3688.35, 1945.45},
    (Vector2){3658.35, 2014.45},
    (Vector2){3718.35, 1995.45},
    (Vector2){3715.35, 1959.45}
);

add_isometric_obstacle(
    (Vector2){2873.43, 2037.39},
    (Vector2){3014.43, 1969.39},
    (Vector2){3019.43, 2016.39},
    (Vector2){2881.43, 2098.39}
);

add_isometric_obstacle(
    (Vector2){1864.78, 1543.99},
    (Vector2){2439.78, 1255.99},
    (Vector2){2468.78, 1265.99},
    (Vector2){1903.78, 1559.99}
);

add_isometric_obstacle(
    (Vector2){1903.78, 1559.99},
    (Vector2){1930.78, 1542.99},
    (Vector2){2024.78, 1591.99},
    (Vector2){1995.78, 1606.99}
);

add_isometric_obstacle(
    (Vector2){1995.78, 1606.99},
    (Vector2){2021.78, 1618.99},
    (Vector2){2084.78, 1590.99},
    (Vector2){2056.78, 1572.99}
);

add_isometric_obstacle(
    (Vector2){2056.78, 1606.99},
    (Vector2){2145.78, 1652.99},
    (Vector2){2212.78, 1620.99},
    (Vector2){2123.78, 1574.99}
);

add_isometric_obstacle(
    (Vector2){2181.78, 1636.99},
    (Vector2){2554.78, 1826.99},
    (Vector2){2581.78, 1807.99},
    (Vector2){2218.78, 1623.99}
);

add_isometric_obstacle(
    (Vector2){2563.78, 1827.99},
    (Vector2){2540.78, 1841.99},
    (Vector2){2658.78, 1909.99},
    (Vector2){2692.78, 1891.99}
);

add_isometric_obstacle(
    (Vector2){2634.78, 1929.99},
    (Vector2){2667.78, 1942.99},
    (Vector2){2727.78, 1911.99},
    (Vector2){2693.78, 1892.99}
);

add_isometric_obstacle(
    (Vector2){2633.83, 1961.43},
    (Vector2){2669.83, 1941.43},
    (Vector2){2851.83, 2036.43},
    (Vector2){2819.83, 2050.43}
);

add_isometric_obstacle(
    (Vector2){2831.83, 2024.43},
    (Vector2){2854.83, 2038.43},
    (Vector2){3364.01, 1787.09},
    (Vector2){3344.01, 1768.09}
);

add_isometric_obstacle(
    (Vector2){3236.01, 1812.09},
    (Vector2){3363.01, 1751.09},
    (Vector2){3336.01, 1733.09},
    (Vector2){3215.01, 1797.09}
);

add_isometric_obstacle(
    (Vector2){3335.04, 1731.75},
    (Vector2){3368.04, 1715.75},
    (Vector2){3241.04, 1651.75},
    (Vector2){3207.04, 1669.75}
);

add_isometric_obstacle(
    (Vector2){3181.04, 1691.75},
    (Vector2){3210.04, 1666.75},
    (Vector2){2635.14, 1380.07},
    (Vector2){2604.14, 1403.07}
);

add_isometric_obstacle(
    (Vector2){2442.97, 1281.14},
    (Vector2){2461.97, 1264.14},
    (Vector2){2563.97, 1315.14},
    (Vector2){2538.97, 1335.14}
);

add_isometric_obstacle(
    (Vector2){2515.97, 1355.14},
    (Vector2){2535.97, 1334.14},
    (Vector2){2624.97, 1377.14},
    (Vector2){2602.97, 1401.14}
);

add_isometric_obstacle(
    (Vector2){1863.97, 1575.14},
    (Vector2){1866.97, 1541.14},
    (Vector2){2021.97, 1618.14},
    (Vector2){2024.97, 1646.14}
);

add_isometric_obstacle(
    (Vector2){2019.97, 1624.14},
    (Vector2){2052.97, 1606.14},
    (Vector2){2058.97, 1636.14},
    (Vector2){2024.97, 1644.14}
);

add_isometric_obstacle(
    (Vector2){2052.97, 1609.14},
    (Vector2){2055.97, 1634.14},
    (Vector2){2151.97, 1688.14},
    (Vector2){2151.97, 1654.14}
);

add_isometric_obstacle(
    (Vector2){2151.97, 1654.14},
    (Vector2){2180.97, 1638.14},
    (Vector2){2184.97, 1668.14},
    (Vector2){2153.97, 1686.14}
);

add_isometric_obstacle(
    (Vector2){2184.97, 1640.14},
    (Vector2){2555.97, 1827.14},
    (Vector2){2538.97, 1845.14},
    (Vector2){2185.97, 1673.14}
);

add_isometric_obstacle(
    (Vector2){2532.97, 1848.14},
    (Vector2){2649.97, 1902.14},
    (Vector2){2632.97, 1923.14},
    (Vector2){2533.97, 1874.14}
);

add_isometric_obstacle(
    (Vector2){2633.97, 1958.14},
    (Vector2){2633.97, 1990.14},
    (Vector2){2825.97, 2081.14},
    (Vector2){2821.97, 2055.14}
);

add_isometric_obstacle(
    (Vector2){2824.31, 2056.63},
    (Vector2){2824.31, 2079.63},
    (Vector2){3366.31, 1817.63},
    (Vector2){3366.31, 1788.63}
);

add_isometric_obstacle(
    (Vector2){1569.16, 1441.69},
    (Vector2){1663.16, 1477.69},
    (Vector2){1663.16, 1409.69},
    (Vector2){1570.16, 1384.69}
);

add_isometric_obstacle(
    (Vector2){1498.16, 1592.69},
    (Vector2){1525.16, 1600.69},
    (Vector2){1524.16, 1635.69},
    (Vector2){1497.16, 1629.69}
);

add_isometric_obstacle(
    (Vector2){1340.16, 1547.69},
    (Vector2){1365.16, 1552.69},
    (Vector2){1368.16, 1521.69},
    (Vector2){1342.16, 1515.69}
);

add_isometric_obstacle(
    (Vector2){1464.16, 1475.69},
    (Vector2){1495.16, 1479.69},
    (Vector2){1497.16, 1457.69},
    (Vector2){1471.16, 1449.69}
);

add_isometric_obstacle(
    (Vector2){1606.16, 1511.69},
    (Vector2){1625.16, 1523.69},
    (Vector2){1613.16, 1546.69},
    (Vector2){1595.16, 1535.69}
);

add_isometric_obstacle(
    (Vector2){1788.16, 1487.69},
    (Vector2){1809.16, 1488.69},
    (Vector2){1820.16, 1458.69},
    (Vector2){1791.16, 1450.69}
);

add_isometric_obstacle(
    (Vector2){1591.16, 1380.69},
    (Vector2){1598.16, 1355.69},
    (Vector2){1628.16, 1359.69},
    (Vector2){1622.16, 1384.69}
);

add_isometric_obstacle(
    (Vector2){2077.16, 1116.69},
    (Vector2){2106.16, 1117.69},
    (Vector2){2102.16, 1145.69},
    (Vector2){2080.16, 1151.69}
);

add_isometric_obstacle(
    (Vector2){2269.16, 1211.69},
    (Vector2){2300.16, 1219.69},
    (Vector2){2292.16, 1246.69},
    (Vector2){2268.16, 1244.69}
);

add_isometric_obstacle(
    (Vector2){1348.16, 1642.69},
    (Vector2){1402.16, 1665.69},
    (Vector2){1391.16, 1699.69},
    (Vector2){1333.16, 1671.69}
);

add_isometric_obstacle(
    (Vector2){1193.16, 1565.69},
    (Vector2){1241.02, 1595.49},
    (Vector2){1230.02, 1621.49},
    (Vector2){1179.02, 1591.49}
);

add_isometric_obstacle(
    (Vector2){1195.02, 1629.49},
    (Vector2){1075.02, 1563.49},
    (Vector2){1072.02, 1594.49},
    (Vector2){1200.02, 1652.49}
);

add_isometric_obstacle(
    (Vector2){1066.02, 1566.49},
    (Vector2){669.024, 1756.49},
    (Vector2){685.024, 1777.49},
    (Vector2){1065.02, 1594.49}
);

add_isometric_obstacle(
    (Vector2){656.024, 1769.49},
    (Vector2){655.024, 1795.49},
    (Vector2){1069.02, 2000.49},
    (Vector2){1077.02, 1976.49}
);

add_isometric_obstacle(
    (Vector2){1077.02, 1970.49},
    (Vector2){1071.02, 2002.49},
    (Vector2){1308.02, 1889.49},
    (Vector2){1309.02, 1856.49}
);

add_isometric_obstacle(
    (Vector2){1315.02, 1803.49},
    (Vector2){1450.02, 1776.49},
    (Vector2){1445.02, 1842.49},
    (Vector2){1318.02, 1886.49}
);

add_isometric_obstacle(
    (Vector2){1447.02, 1788.49},
    (Vector2){1483.02, 1772.49},
    (Vector2){1490.02, 1793.49},
    (Vector2){1450.02, 1812.49}
);

add_isometric_obstacle(
    (Vector2){1487.02, 1767.49},
    (Vector2){1307.02, 1681.49},
    (Vector2){1302.02, 1702.49},
    (Vector2){1453.02, 1777.49}
);

add_isometric_obstacle(
    (Vector2){1001.02, 1653.49},
    (Vector2){1158.02, 1736.49},
    (Vector2){1131.02, 1779.49},
    (Vector2){970.024, 1701.49}
);

add_isometric_obstacle(
    (Vector2){970.024, 1701.49},
    (Vector2){968.024, 1760.49},
    (Vector2){1131.02, 1840.49},
    (Vector2){1127.02, 1780.49}
);

add_isometric_obstacle(
    (Vector2){1127.02, 1780.49},
    (Vector2){1185.02, 1753.49},
    (Vector2){1199.02, 1819.49},
    (Vector2){1134.02, 1844.49}
);

add_isometric_obstacle(
    (Vector2){922.024, 1733.49},
    (Vector2){949.024, 1723.49},
    (Vector2){952.024, 1748.49},
    (Vector2){932.024, 1757.49}
);



add_isometric_obstacle(
    (Vector2){4890.13, 3100.81},
    (Vector2){4891.13, 3070.81},
    (Vector2){4919.13, 3075.81},
    (Vector2){4919.13, 3098.81}
);

add_isometric_obstacle(
    (Vector2){4984.13, 3187.81},
    (Vector2){5009.13, 3180.81},
    (Vector2){5019.13, 3206.81},
    (Vector2){4990.13, 3216.81}
);

add_isometric_obstacle(
    (Vector2){5144.13, 3229.81},
    (Vector2){5178.13, 3232.81},
    (Vector2){5176.13, 3259.81},
    (Vector2){5151.13, 3255.81}
);

add_isometric_obstacle(
    (Vector2){5339.13, 3263.81},
    (Vector2){5359.13, 3261.81},
    (Vector2){5371.13, 3279.81},
    (Vector2){5348.13, 3292.81}
);

add_isometric_obstacle(
    (Vector2){5531.13, 3203.81},
    (Vector2){5554.13, 3198.81},
    (Vector2){5561.13, 3222.81},
    (Vector2){5537.13, 3229.81}
);

add_isometric_obstacle(
    (Vector2){5599.13, 3145.81},
    (Vector2){5622.13, 3134.81},
    (Vector2){5622.13, 3162.81},
    (Vector2){5599.13, 3164.81}
);

add_isometric_obstacle(
    (Vector2){5244.02, 2919.14},
    (Vector2){5386.02, 2846.14},
    (Vector2){5736.02, 3026.14},
    (Vector2){5599.02, 3100.14}
);

add_isometric_obstacle(
    (Vector2){5246.02, 3262.14},
    (Vector2){5269.02, 3261.14},
    (Vector2){5278.02, 3231.14},
    (Vector2){5245.02, 3229.14}
);

add_isometric_obstacle(
    (Vector2){5671.41, 3088.09},
    (Vector2){5693.41, 3057.09},
    (Vector2){5725.41, 3062.09},
    (Vector2){5705.41, 3103.09}
);

add_isometric_obstacle(
    (Vector2){5249.41, 2821.09},
    (Vector2){5288.41, 2841.09},
    (Vector2){5260.41, 2885.09},
    (Vector2){5228.41, 2866.09}
);

add_isometric_obstacle(
    (Vector2){6092.39, 2982.63},
    (Vector2){6087.39, 3044.63},
    (Vector2){6215.39, 3107.63},
    (Vector2){6218.39, 3044.63}
);

add_isometric_obstacle(
    (Vector2){6218.39, 3044.63},
    (Vector2){6493.39, 2901.63},
    (Vector2){6501.39, 2962.63},
    (Vector2){6220.39, 3103.63}
);

add_isometric_obstacle(
    (Vector2){6318.39, 2866.63},
    (Vector2){6435.39, 2931.63},
    (Vector2){6494.39, 2898.63},
    (Vector2){6347.39, 2821.63}
);

add_isometric_obstacle(
    (Vector2){6577.01, 3320.72},
    (Vector2){6577.01, 3340.72},
    (Vector2){5392.11, 2752.97},
    (Vector2){5392.11, 2735.97}
);

add_isometric_obstacle(
    (Vector2){5392.11, 2735.97},
    (Vector2){6173.11, 2339.97},
    (Vector2){6180.11, 2360.97},
    (Vector2){5424.11, 2738.97}
);

add_isometric_obstacle(
    (Vector2){6255.5, 2294.51},
    (Vector2){6257.5, 2319.51},
    (Vector2){6926.5, 1992.51},
    (Vector2){6925.5, 1962.51}
);

add_isometric_obstacle(
    (Vector2){6925.5, 1962.51},
    (Vector2){8113.74, 2553.02},
    (Vector2){8111.74, 2577.02},
    (Vector2){6917.98, 1987.48}
);

add_isometric_obstacle(
    (Vector2){7060.98, 1924.48},
    (Vector2){7067.98, 2017.48},
    (Vector2){7237.98, 2009.48},
    (Vector2){7237.98, 1924.48}
);

add_isometric_obstacle(
    (Vector2){6089.98, 2753.48},
    (Vector2){6978.77, 2308.39},
    (Vector2){6938.77, 2285.39},
    (Vector2){6060.77, 2740.39}
);

add_isometric_obstacle(
    (Vector2){6796.77, 2247.39},
    (Vector2){6861.77, 2248.39},
    (Vector2){6925.77, 2281.39},
    (Vector2){6896.77, 2303.39}
);

add_isometric_obstacle(
    (Vector2){6797.77, 2246.39},
    (Vector2){6942.77, 2160.39},
    (Vector2){6973.77, 2175.39},
    (Vector2){6857.77, 2247.39}
);

add_isometric_obstacle(
    (Vector2){6915.77, 2182.39},
    (Vector2){6858.77, 2150.39},
    (Vector2){6853.77, 2119.39},
    (Vector2){6965.77, 2173.39}
);

add_isometric_obstacle(
    (Vector2){6855.77, 2155.39},
    (Vector2){5730.77, 2708.39},
    (Vector2){5671.77, 2707.39},
    (Vector2){6850.77, 2118.39}
);

add_isometric_obstacle(
    (Vector2){5673.77, 2708.39},
    (Vector2){5795.77, 2772.39},
    (Vector2){5803.77, 2743.39},
    (Vector2){5731.77, 2708.39}
);

add_isometric_obstacle(
    (Vector2){5810.77, 2739.39},
    (Vector2){5930.77, 2667.39},
    (Vector2){5937.77, 2700.39},
    (Vector2){5796.77, 2766.39}
);

add_isometric_obstacle(
    (Vector2){5935.77, 2670.39},
    (Vector2){5999.77, 2645.39},
    (Vector2){6056.77, 2724.39},
    (Vector2){5978.77, 2747.39}
);

add_isometric_obstacle(
    (Vector2){5935.77, 2728.39},
    (Vector2){5939.77, 2611.39},
    (Vector2){6057.77, 2680.39},
    (Vector2){6055.77, 2765.39}
);

add_isometric_obstacle(
    (Vector2){5799.46, 2769.93},
    (Vector2){5803.46, 2801.93},
    (Vector2){5940.46, 2737.93},
    (Vector2){5933.46, 2711.93}
);

add_isometric_obstacle(
    (Vector2){6062.36, 2745.38},
    (Vector2){6061.36, 2775.38},
    (Vector2){6089.36, 2785.38},
    (Vector2){6090.36, 2749.38}
);

add_isometric_obstacle(
    (Vector2){6090.36, 2749.38},
    (Vector2){6092.36, 2785.38},
    (Vector2){6982.36, 2344.38},
    (Vector2){6982.36, 2310.38}
);

add_isometric_obstacle(
    (Vector2){6893.36, 2259.38},
    (Vector2){6984.36, 2211.38},
    (Vector2){6976.36, 2188.38},
    (Vector2){6873.36, 2247.38}
);

add_isometric_obstacle(
    (Vector2){6429.1, 2703.23},
    (Vector2){7021.1, 2396.23},
    (Vector2){7172.1, 2467.23},
    (Vector2){6567.1, 2771.23}
);

add_isometric_obstacle(
    (Vector2){6567.1, 2771.23},
    (Vector2){6571.1, 2864.23},
    (Vector2){7176.1, 2564.23},
    (Vector2){7171.1, 2470.23}
);

add_isometric_obstacle(
    (Vector2){6423.1, 2704.23},
    (Vector2){6409.1, 2782.23},
    (Vector2){6565.1, 2867.23},
    (Vector2){6562.1, 2765.23}
);

add_isometric_obstacle(
    (Vector2){6595.75, 2947.38},
    (Vector2){6787.69, 3055.01},
    (Vector2){7603.21, 2644.54},
    (Vector2){7395.21, 2546.54}
);

add_isometric_obstacle(
    (Vector2){7326.21, 2457.54},
    (Vector2){7335.21, 2504.54},
    (Vector2){7401.21, 2492.54},
    (Vector2){7400.21, 2449.54}
);

add_isometric_obstacle(
    (Vector2){7417.21, 2443.54},
    (Vector2){7426.21, 2476.54},
    (Vector2){7447.21, 2478.54},
    (Vector2){7447.21, 2453.54}
);

add_isometric_obstacle(
    (Vector2){7496.46, 2467.46},
    (Vector2){7596.46, 2354.46},
    (Vector2){7884.46, 2504.46},
    (Vector2){7787.46, 2575.46}
);

add_isometric_obstacle(
    (Vector2){7785.46, 2576.46},
    (Vector2){7786.46, 2601.46},
    (Vector2){7909.46, 2551.46},
    (Vector2){7903.46, 2521.46}
);

add_isometric_obstacle(
    (Vector2){6088.56, 2981.91},
    (Vector2){6215.56, 3043.91},
    (Vector2){6214.56, 3105.91},
    (Vector2){6087.56, 3044.91}
);

add_isometric_obstacle(
    (Vector2){6214.56, 3038.91},
    (Vector2){6500.56, 2897.91},
    (Vector2){6501.56, 2964.91},
    (Vector2){6220.56, 3102.91}
);

add_isometric_obstacle(
    (Vector2){6089.56, 2977.91},
    (Vector2){6341.56, 2819.91},
    (Vector2){6494.56, 2900.91},
    (Vector2){6211.56, 3030.91}
);

add_isometric_obstacle(
    (Vector2){7772.46, 2574.46},
    (Vector2){7500.46, 2434.46},
    (Vector2){7500.46, 2461.46},
    (Vector2){7780.46, 2597.46}
);


}
