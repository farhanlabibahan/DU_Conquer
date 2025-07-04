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
    float radius = 10.0f;
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
        (Vector2){4967.31f, 2671.55f},
        (Vector2){5002.31f, 2688.55f},
        (Vector2){5762.34f, 2309.97f},
        (Vector2){5758.01f, 2280.64f}
    );

    add_isometric_obstacle(
        (Vector2){5827.1f, 2243.71f},
        (Vector2){5828.1f, 2271.37f},
        (Vector2){6501.24f, 1941.84f},
        (Vector2){6499.58f, 1907.51f}
    );

    add_isometric_obstacle(
        (Vector2){6503.08f, 1908.14f},
        (Vector2){6502.08f, 1937.14f},
        (Vector2){7690.66f, 2528.71f},
        (Vector2){7693.66f, 2500.71f}
    );

    add_isometric_obstacle(
        (Vector2){4966.62f, 2675.63f},
        (Vector2){4965.62f, 2698.3f},
        (Vector2){6149.94f, 3292.58f},
        (Vector2){6151.61f, 3263.58f}
    );

    add_isometric_obstacle(
        (Vector2){6367.2f, 3005.97f},
        (Vector2){7191.82f, 2593.68f},
        (Vector2){6974.82f, 2491.34f},
        (Vector2){6149.6f, 2897.24f}
    );

    add_isometric_obstacle(
        (Vector2){3806.51f, 2627.63f},
        (Vector2){3808.51f, 2445.3f},
        (Vector2){3886.18f, 2484.63f},
        (Vector2){3887.51f, 2667.96f}
    );

    add_isometric_obstacle(
        (Vector2){3886.85f, 2482.96f},
        (Vector2){3966.18f, 2446.96f},
        (Vector2){3967.18f, 2627.96f},
        (Vector2){3887.85f, 2666.63f}
    );

    add_isometric_obstacle(
        (Vector2){3808.85f, 2443.96f},
        (Vector2){3886.51f, 2405.96f},
        (Vector2){3965.51f, 2444.3f},
        (Vector2){3885.85f, 2483.63f}
    );

    add_isometric_obstacle(
        (Vector2){4000.18f, 2531.88f},
        (Vector2){4000.85f, 2353.55f},
        (Vector2){4079.18f, 2391.22f},
        (Vector2){4158.51f, 2351.88f}
    );

    add_isometric_obstacle(
        (Vector2){4000.18f, 2531.88f},
        (Vector2){4000.18f, 2352.22f},
        (Vector2){4079.18f, 2389.55f},
        (Vector2){4078.85f, 2571.55f}
    );

    add_isometric_obstacle(
        (Vector2){4080.85f, 2571.22f},
        (Vector2){4079.51f, 2388.55f},
        (Vector2){4158.18f, 2351.55f},
        (Vector2){4157.85f, 2533.55f}
    );

    add_isometric_obstacle(
        (Vector2){4080.18f, 2390.88f},
        (Vector2){4157.51f, 2351.55f},
        (Vector2){4087.18f, 2308.22f},
        (Vector2){4000.85f, 2350.55f}
    );

    add_isometric_obstacle(
        (Vector2){3857.17f, 2938.38f},
        (Vector2){3855.17f, 2759.38f},
        (Vector2){3903.84f, 2783.05f},
        (Vector2){3902.5f, 2961.71f}
    );

    add_isometric_obstacle(
        (Vector2){3902.5f, 2961.71f},
        (Vector2){3902.17f, 2782.38f},
        (Vector2){3938.17f, 2784.05f},
        (Vector2){3937.17f, 2962.38f}
    );

    add_isometric_obstacle(
        (Vector2){3937.17f, 2962.38f},
        (Vector2){3937.17f, 2782.71f},
        (Vector2){3984.5f, 2760.05f},
        (Vector2){3984.5f, 2938.38f}
    );

    add_isometric_obstacle(
        (Vector2){3855.17f, 2758.71f},
        (Vector2){3855.17f, 2742.05f},
        (Vector2){3936.5f, 2780.38f},
        (Vector2){3902.84f, 2781.05f}
    );

    add_isometric_obstacle(
        (Vector2){3857.5f, 2741.38f},
        (Vector2){3902.17f, 2717.71f},
        (Vector2){3983.17f, 2757.38f},
        (Vector2){3936.5f, 2781.38f}
    );

    add_isometric_obstacle(
        (Vector2){3902.84f, 2718.38f},
        (Vector2){3938.84f, 2717.05f},
        (Vector2){3983.17f, 2742.05f},
        (Vector2){3983.84f, 2757.71f}
    );

    add_isometric_obstacle(
        (Vector2){4329.26f, 2958.82f},
        (Vector2){4331.26f, 2932.49f},
        (Vector2){3984.69f, 2758.66f},
        (Vector2){3984.36f, 2788.66f}
    );

    add_isometric_obstacle(
        (Vector2){3899.76f, 2715.41f},
        (Vector2){3761.42f, 2648.08f},
        (Vector2){3760.42f, 2675.74f},
        (Vector2){3875.42f, 2730.74f}
    );

    add_isometric_obstacle(
        (Vector2){3630.7f, 2650.33f},
        (Vector2){3679.03f, 2671.66f},
        (Vector2){3712.03f, 2672.66f},
        (Vector2){3631.03f, 2630.66f}
    );

    add_isometric_obstacle(
        (Vector2){3631.03f, 2630.66f},
        (Vector2){3676.7f, 2608.33f},
        (Vector2){3758.7f, 2646.66f},
        (Vector2){3713.03f, 2672.33f}
    );

    add_isometric_obstacle(
        (Vector2){3676.37f, 2604.0f},
        (Vector2){3713.37f, 2607.66f},
        (Vector2){3759.03f, 2629.0f},
        (Vector2){3758.37f, 2646.66f}
    );

    add_isometric_obstacle(
        (Vector2){3632.69f, 2650.42f},
        (Vector2){3632.06f, 2826.58f},
        (Vector2){3679.73f, 2849.58f},
        (Vector2){3679.73f, 2672.58f}
    );

    add_isometric_obstacle(
        (Vector2){3679.4f, 2669.25f},
        (Vector2){3711.4f, 2671.25f},
        (Vector2){3711.73f, 2848.92f},
        (Vector2){3680.06f, 2849.58f}
    );

    add_isometric_obstacle(
        (Vector2){3711.06f, 2670.58f},
        (Vector2){3759.06f, 2648.25f},
        (Vector2){3760.06f, 2827.58f},
        (Vector2){3712.73f, 2850.25f}
    );

    add_isometric_obstacle(
        (Vector2){3496.89f, 2705.6f},
        (Vector2){3629.22f, 2639.27f},
        (Vector2){3630.22f, 2665.27f},
        (Vector2){3493.56f, 2733.27f}
    );

    add_isometric_obstacle(
        (Vector2){3430.89f, 2740.6f},
        (Vector2){3429.91f, 2767.69f},
        (Vector2){4072.79f, 3086.35f},
        (Vector2){4075.46f, 3060.35f}
    );

    add_isometric_obstacle(
        (Vector2){3497.08f, 2707.47f},
        (Vector2){3492.75f, 2732.8f},
        (Vector2){2951.07f, 2464.62f},
        (Vector2){2950.41f, 2433.95f}
    );

    add_isometric_obstacle(
        (Vector2){2951.03f, 2433.95f},
        (Vector2){2980.7f, 2448.62f},
        (Vector2){3167.7f, 2354.28f},
        (Vector2){3168.03f, 2326.95f}
    );

    add_isometric_obstacle(
        (Vector2){3458.54f, 2181.63f},
        (Vector2){3460.87f, 2207.63f},
        (Vector2){3621.54f, 2128.63f},
        (Vector2){3622.54f, 2102.63f}
    );

    add_isometric_obstacle(
        (Vector2){3621.54f, 2129.16f},
        (Vector2){3623.54f, 2102.16f},
        (Vector2){4012.09f, 2289.82f},
        (Vector2){4010.09f, 2318.16f}
    );

    add_isometric_obstacle(
        (Vector2){2944.17f, 1791.46f},
        (Vector2){3232.17f, 1935.12f},
        (Vector2){3231.5f, 1967.12f},
        (Vector2){2943.5f, 1824.46f}
    );

    add_isometric_obstacle(
        (Vector2){2943.5f, 1789.46f},
        (Vector2){2976.5f, 1744.79f},
        (Vector2){3262.29f, 1886.95f},
        (Vector2){3231.95f, 1932.95f}
    );

    add_isometric_obstacle(
        (Vector2){3231.95f, 1932.95f},
        (Vector2){3263.95f, 1887.62f},
        (Vector2){3232.62f, 1965.95f},
        (Vector2){3275.29f, 1942.95f}
    );

    add_isometric_obstacle(
        (Vector2){3231.95f, 1932.28f},
        (Vector2){3263.29f, 1887.28f},
        (Vector2){3273.95f, 1940.62f},
        (Vector2){3233.29f, 1965.62f}
    );

    add_isometric_obstacle(
        (Vector2){3263.29f, 1887.95f},
        (Vector2){3295.29f, 1902.28f},
        (Vector2){3294.95f, 1935.28f},
        (Vector2){3274.62f, 1942.95f}
    );

    add_isometric_obstacle(
        (Vector2){232.519f, 1714.27f},
        (Vector2){259.853f, 1727.94f},
        (Vector2){644.186f, 1535.94f},
        (Vector2){645.935f, 1507.03f}
    );

    add_isometric_obstacle(
        (Vector2){646.231f, 1506.28f},
        (Vector2){646.231f, 1534.28f},
        (Vector2){778.897f, 1596.61f},
        (Vector2){779.564f, 1571.94f}
    );

    add_isometric_obstacle(
        (Vector2){870.897f, 1646.94f},
        (Vector2){873.897f, 1621.94f},
        (Vector2){1058.63f, 1711.2f},
        (Vector2){1055.63f, 1739.2f}
    );

    add_isometric_obstacle(
        (Vector2){647.482f, 1920.97f},
        (Vector2){647.482f, 1948.63f},
        (Vector2){1056.48f, 1747.63f},
        (Vector2){1055.48f, 1720.97f}
    );

    add_isometric_obstacle(
        (Vector2){645.087f, 1947.71f},
        (Vector2){646.754f, 1921.38f},
        (Vector2){231.738f, 1714.54f},
        (Vector2){230.405f, 1741.87f}
    );

    add_isometric_obstacle(
        (Vector2){5249.3f, 2654.3f},
        (Vector2){5376.3f, 2716.3f},
        (Vector2){6555.52f, 2125.17f},
        (Vector2){6431.52f, 2057.17f}
    );

    add_isometric_obstacle(
        (Vector2){5532.52f, 2640.17f},
        (Vector2){6434.52f, 2188.17f},
        (Vector2){6556.52f, 2250.17f},
        (Vector2){5666.52f, 2696.17f}
    );

    add_isometric_obstacle(
        (Vector2){5666.52f, 2696.17f},
        (Vector2){5662.52f, 2728.17f},
        (Vector2){6559.52f, 2281.17f},
        (Vector2){6554.52f, 2247.17f}
    );

    add_isometric_obstacle(
        (Vector2){6434.52f, 2187.17f},
        (Vector2){6554.52f, 2122.17f},
        (Vector2){6557.52f, 2153.17f},
        (Vector2){6467.52f, 2203.17f}
    );

    add_isometric_obstacle(
        (Vector2){5531.52f, 2640.17f},
        (Vector2){5535.52f, 2668.17f},
        (Vector2){5654.52f, 2727.17f},
        (Vector2){5663.52f, 2692.17f}
    );

    add_isometric_obstacle(
        (Vector2){5537.52f, 2665.17f},
        (Vector2){5376.52f, 2748.17f},
        (Vector2){5373.52f, 2709.17f},
        (Vector2){5528.52f, 2636.17f}
    );

    add_isometric_obstacle(
        (Vector2){5374.52f, 2742.17f},
        (Vector2){5373.52f, 2709.17f},
        (Vector2){5247.52f, 2653.17f},
        (Vector2){5245.52f, 2683.17f}
    );

    add_isometric_obstacle(
        (Vector2){5664.3f, 2928.16f},
        (Vector2){5790.3f, 2982.16f},
        (Vector2){5790.3f, 3052.16f},
        (Vector2){5661.3f, 2993.16f}
    );

    add_isometric_obstacle(
        (Vector2){5789.3f, 2984.16f},
        (Vector2){6077.3f, 2844.16f},
        (Vector2){6077.3f, 2903.16f},
        (Vector2){5792.3f, 3049.16f}
    );

    add_isometric_obstacle(
        (Vector2){5790.3f, 2977.16f},
        (Vector2){5659.3f, 2925.16f},
        (Vector2){5915.3f, 2766.16f},
        (Vector2){6074.3f, 2845.16f}
    );

    add_isometric_obstacle(
        (Vector2){5658.3f, 2918.16f},
        (Vector2){5691.3f, 2870.16f},
        (Vector2){5826.3f, 2938.16f},
        (Vector2){5790.3f, 2982.16f}
    );

    add_isometric_obstacle(
        (Vector2){5981.79f, 2728.74f},
        (Vector2){5983.79f, 2644.74f},
        (Vector2){6142.79f, 2714.74f},
        (Vector2){6141.79f, 2809.74f}
    );

    add_isometric_obstacle(
        (Vector2){6137.79f, 2708.74f},
        (Vector2){6749.79f, 2414.74f},
        (Vector2){6748.79f, 2503.74f},
        (Vector2){6141.79f, 2809.74f}
    );

    add_isometric_obstacle(
        (Vector2){5985.79f, 2634.74f},
        (Vector2){6593.79f, 2327.74f},
        (Vector2){6742.79f, 2408.74f},
        (Vector2){6138.79f, 2707.74f}
    );

    add_isometric_obstacle(
        (Vector2){6942.01f, 2403.49f},
        (Vector2){7204.01f, 2268.49f},
        (Vector2){7202.01f, 2305.49f},
        (Vector2){6977.01f, 2424.49f}
    );

    add_isometric_obstacle(
        (Vector2){7204.01f, 2267.49f},
        (Vector2){7617.01f, 2481.49f},
        (Vector2){7545.01f, 2475.49f},
        (Vector2){7202.01f, 2304.49f}
    );

    add_isometric_obstacle(
        (Vector2){7329.01f, 2595.49f},
        (Vector2){7360.01f, 2610.49f},
        (Vector2){7613.01f, 2482.49f},
        (Vector2){7546.01f, 2478.49f}
    );

    add_isometric_obstacle(
        (Vector2){7357.01f, 2528.49f},
        (Vector2){7481.01f, 2460.49f},
        (Vector2){7170.01f, 2299.49f},
        (Vector2){7075.01f, 2381.49f}
    );

    add_isometric_obstacle(
        (Vector2){7075.01f, 2381.49f},
        (Vector2){7072.01f, 2416.49f},
        (Vector2){7360.01f, 2555.49f},
        (Vector2){7359.01f, 2525.49f}
    );

    add_isometric_obstacle(
        (Vector2){7359.01f, 2525.49f},
        (Vector2){7485.01f, 2459.49f},
        (Vector2){7488.01f, 2488.49f},
        (Vector2){7361.01f, 2551.49f}
    );

    add_isometric_obstacle(
        (Vector2){7301.01f, 2593.49f},
        (Vector2){7328.01f, 2604.49f},
        (Vector2){7308.01f, 2538.49f},
        (Vector2){7352.01f, 2552.49f}
    );

    add_isometric_obstacle(
        (Vector2){7352.01f, 2552.49f},
        (Vector2){7331.01f, 2598.49f},
        (Vector2){7301.01f, 2589.49f},
        (Vector2){7308.01f, 2539.49f}
    );

    add_isometric_obstacle(
        (Vector2){6980.01f, 2419.49f},
        (Vector2){7005.01f, 2436.49f},
        (Vector2){7027.01f, 2415.49f},
        (Vector2){7027.01f, 2396.49f}
    );

    add_isometric_obstacle(
        (Vector2){5201.16f, 3064.01f},
        (Vector2){5350.16f, 2987.01f},
        (Vector2){4940.16f, 2776.01f},
        (Vector2){4798.16f, 2858.01f}
    );

    add_isometric_obstacle(
        (Vector2){4819.95f, 2870.52f},
        (Vector2){5174.95f, 3047.52f},
        (Vector2){5171.95f, 3073.52f},
        (Vector2){4810.95f, 2895.52f}
    );

    add_isometric_obstacle(
        (Vector2){4992.95f, 2798.52f},
        (Vector2){5010.95f, 2775.52f},
        (Vector2){5302.95f, 2924.52f},
        (Vector2){5291.95f, 2947.52f}
    );

    add_isometric_obstacle(
        (Vector2){5103.95f, 3191.52f},
        (Vector2){5103.95f, 3191.52f},
        (Vector2){5099.95f, 3134.52f},
        (Vector2){5139.95f, 3134.52f}
    );

    add_isometric_obstacle(
        (Vector2){5104.95f, 3188.52f},
        (Vector2){5100.95f, 3133.52f},
        (Vector2){5135.95f, 3132.52f},
        (Vector2){5139.95f, 3179.52f}
    );

    add_isometric_obstacle(
        (Vector2){5166.95f, 3126.52f},
        (Vector2){5162.95f, 3071.52f},
        (Vector2){5201.95f, 3072.52f},
        (Vector2){5202.95f, 3118.52f}
    );

    add_isometric_obstacle(
        (Vector2){4947.76f, 3195.75f},
        (Vector2){4947.76f, 3195.75f},
        (Vector2){4947.76f, 3195.75f},
        (Vector2){4906.76f, 3191.75f}
    );

    add_isometric_obstacle(
        (Vector2){4905.76f, 3252.75f},
        (Vector2){4904.76f, 3192.75f},
        (Vector2){4943.76f, 3191.75f},
        (Vector2){4948.76f, 3241.75f}
    );

    add_isometric_obstacle(
        (Vector2){4717.76f, 3215.75f},
        (Vector2){4715.76f, 3168.75f},
        (Vector2){4754.76f, 3170.75f},
        (Vector2){4755.76f, 3212.75f}
    );

    add_isometric_obstacle(
        (Vector2){4556.76f, 3166.75f},
        (Vector2){4556.76f, 3118.75f},
        (Vector2){4594.76f, 3118.75f},
        (Vector2){4593.76f, 3171.75f}
    );

    add_isometric_obstacle(
        (Vector2){4454.76f, 3046.75f},
        (Vector2){4489.76f, 3058.75f},
        (Vector2){4463.76f, 2997.75f},
        (Vector2){4498.76f, 3006.75f}
    );

    add_isometric_obstacle(
        (Vector2){4498.76f, 3006.75f},
        (Vector2){4464.76f, 2996.75f},
        (Vector2){4453.76f, 3040.75f},
        (Vector2){4485.76f, 3051.75f}
    );

    add_isometric_obstacle(
        (Vector2){4571.76f, 3229.75f},
        (Vector2){4603.76f, 3243.75f},
        (Vector2){4605.76f, 3209.75f},
        (Vector2){4635.76f, 3227.75f}
    );

    add_isometric_obstacle(
        (Vector2){4577.76f, 3228.75f},
        (Vector2){4603.76f, 3246.75f},
        (Vector2){4633.76f, 3230.75f},
        (Vector2){4603.76f, 3204.75f}
    );

    add_isometric_obstacle(
        (Vector2){4669.76f, 3247.75f},
        (Vector2){4699.76f, 3228.75f},
        (Vector2){4729.76f, 3240.75f},
        (Vector2){4708.76f, 3257.75f}
    );

    add_isometric_obstacle(
        (Vector2){4772.76f, 3260.75f},
        (Vector2){4799.76f, 3243.75f},
        (Vector2){4822.76f, 3255.75f},
        (Vector2){4797.76f, 3273.75f}
    );

    add_isometric_obstacle(
        (Vector2){3440.77f, 2660.16f},
        (Vector2){3555.45f, 2605.4f},
        (Vector2){3220.45f, 2428.4f},
        (Vector2){3105.45f, 2487.4f}
    );

    add_isometric_obstacle(
        (Vector2){3230.45f, 2415.4f},
        (Vector2){3230.45f, 2381.4f},
        (Vector2){3453.45f, 2497.4f},
        (Vector2){3454.45f, 2524.4f}
    );

    add_isometric_obstacle(
        (Vector2){3454.45f, 2524.4f},
        (Vector2){3454.45f, 2493.4f},
        (Vector2){3613.45f, 2413.4f},
        (Vector2){3615.45f, 2439.4f}
    );

    add_isometric_obstacle(
        (Vector2){3454.45f, 2490.4f},
        (Vector2){3611.45f, 2413.4f},
        (Vector2){3391.45f, 2297.4f},
        (Vector2){3231.45f, 2380.4f}
    );

    add_isometric_obstacle(
        (Vector2){3265.45f, 2357.4f},
        (Vector2){3295.45f, 2313.4f},
        (Vector2){3305.45f, 2330.4f},
        (Vector2){3295.45f, 2341.4f}
    );

    add_isometric_obstacle(
        (Vector2){3334.45f, 2321.4f},
        (Vector2){3356.45f, 2283.4f},
        (Vector2){3370.45f, 2302.4f},
        (Vector2){3358.45f, 2313.4f}
    );

    add_isometric_obstacle(
        (Vector2){3520.45f, 2279.4f},
        (Vector2){3741.45f, 2391.4f},
        (Vector2){3741.45f, 2410.4f},
        (Vector2){3520.45f, 2299.4f}
    );

    add_isometric_obstacle(
        (Vector2){3739.45f, 2387.4f},
        (Vector2){3900.45f, 2310.4f},
        (Vector2){3901.45f, 2329.4f},
        (Vector2){3743.45f, 2408.4f}
    );

    add_isometric_obstacle(
        (Vector2){3522.45f, 2275.4f},
        (Vector2){3678.45f, 2186.4f},
        (Vector2){3903.45f, 2307.4f},
        (Vector2){3741.45f, 2387.4f}
    );

    add_isometric_obstacle(
        (Vector2){4125.87f, 2633.25f},
        (Vector2){4291.87f, 2528.25f},
        (Vector2){4667.87f, 2737.25f},
        (Vector2){4514.87f, 2826.25f}
    );

    add_isometric_obstacle(
        (Vector2){2569.19f, 2363.42f},
        (Vector2){2805.58f, 2479.07f},
        (Vector2){3614.58f, 2076.07f},
        (Vector2){3357.58f, 1954.07f}
    );

    add_isometric_obstacle(
        (Vector2){1568.41f, 1822.33f},
        (Vector2){1729.41f, 1910.33f},
        (Vector2){1886.41f, 1809.33f},
        (Vector2){1731.41f, 1729.33f}
    );

    add_isometric_obstacle(
        (Vector2){1363.14f, 1484.97f},
        (Vector2){1088.14f, 1344.97f},
        (Vector2){1320.14f, 1232.97f},
        (Vector2){1563.14f, 1363.97f}
    );

    add_isometric_obstacle(
        (Vector2){1421.14f, 1173.97f},
        (Vector2){1665.14f, 1307.97f},
        (Vector2){1662.14f, 1053.97f},
        (Vector2){1895.14f, 1188.97f}
    );

    add_isometric_obstacle(
        (Vector2){1895.14f, 1188.97f},
        (Vector2){1665.14f, 1301.97f},
        (Vector2){1430.14f, 1167.97f},
        (Vector2){1663.14f, 1051.97f}
    );

    add_isometric_obstacle(
        (Vector2){612.137f, 1596.97f},
        (Vector2){883.137f, 1736.97f},
        (Vector2){690.137f, 1837.97f},
        (Vector2){425.137f, 1700.97f}
    );

    add_isometric_obstacle(
        (Vector2){563.137f, 1620.97f},
        (Vector2){573.137f, 1597.97f},
        (Vector2){588.137f, 1606.97f},
        (Vector2){563.137f, 1620.97f}
    );

    add_isometric_obstacle(
        (Vector2){794.137f, 1471.97f},
        (Vector2){795.137f, 1469.97f},
        (Vector2){795.137f, 1469.97f},
        (Vector2){795.137f, 1469.97f}
    );

    add_isometric_obstacle(
        (Vector2){795.137f, 1469.97f},
        (Vector2){795.137f, 1469.97f},
        (Vector2){805.137f, 1462.97f},
        (Vector2){801.137f, 1464.97f}
    );

    add_isometric_obstacle(
        (Vector2){902.781f, 1623.29f},
        (Vector2){956.781f, 1656.29f},
        (Vector2){976.781f, 1614.29f},
        (Vector2){925.781f, 1583.29f}
    );

    add_isometric_obstacle(
        (Vector2){790.781f, 1571.29f},
        (Vector2){816.781f, 1529.29f},
        (Vector2){760.781f, 1502.29f},
        (Vector2){742.781f, 1536.29f}
    );

    add_isometric_obstacle(
        (Vector2){2400.01f, 1997.5f},
        (Vector2){2941.07f, 1730.31f},
        (Vector2){2905.07f, 1710.31f},
        (Vector2){2370.07f, 1984.31f}
    );

    add_isometric_obstacle(
        (Vector2){2370.07f, 1984.31f},
        (Vector2){2955.07f, 1685.31f},
        (Vector2){2815.07f, 1595.31f},
        (Vector2){2209.07f, 1902.31f}
    );

    add_isometric_obstacle(
        (Vector2){2209.07f, 1902.31f},
        (Vector2){2771.7f, 1605.18f},
        (Vector2){2736.7f, 1589.18f},
        (Vector2){2210.7f, 1867.18f}
    );

    add_isometric_obstacle(
        (Vector2){2230.7f, 1852.18f},
        (Vector2){2111.7f, 1794.18f},
        (Vector2){2597.7f, 1520.18f},
        (Vector2){2721.7f, 1582.18f}
    );

    add_isometric_obstacle(
        (Vector2){2137.7f, 1775.18f},
        (Vector2){2593.7f, 1518.18f},
        (Vector2){2339.7f, 1393.18f},
        (Vector2){1884.7f, 1645.18f}
    );

    add_isometric_obstacle(
        (Vector2){1884.7f, 1645.18f},
        (Vector2){1760.7f, 1586.18f},
        (Vector2){2206.7f, 1332.18f},
        (Vector2){2338.7f, 1391.18f}
    );

    add_isometric_obstacle(
        (Vector2){1761.7f, 1583.18f},
        (Vector2){1761.7f, 1583.18f},
        (Vector2){1761.7f, 1583.18f},
        (Vector2){1761.7f, 1583.18f}
    );

    add_isometric_obstacle(
        (Vector2){1721.7f, 1597.18f},
        (Vector2){1632.7f, 1550.18f},
        (Vector2){2122.7f, 1285.18f},
        (Vector2){2200.7f, 1322.18f}
    );

    add_isometric_obstacle(
        (Vector2){1600.7f, 1567.18f},
        (Vector2){2141.7f, 1266.18f},
        (Vector2){2018.74f, 1198.39f},
        (Vector2){1439.74f, 1487.39f}
    );

    add_isometric_obstacle(
        (Vector2){1439.74f, 1487.39f},
        (Vector2){1439.74f, 1512.39f},
        (Vector2){1601.74f, 1597.39f},
        (Vector2){1596.74f, 1566.39f}
    );

    add_isometric_obstacle(
        (Vector2){1599.74f, 1568.39f},
        (Vector2){1601.74f, 1595.39f},
        (Vector2){1636.74f, 1584.39f},
        (Vector2){1633.74f, 1556.39f}
    );

    add_isometric_obstacle(
        (Vector2){1632.74f, 1556.39f},
        (Vector2){1726.74f, 1601.39f},
        (Vector2){1726.74f, 1630.39f},
        (Vector2){1635.74f, 1579.39f}
    );

    add_isometric_obstacle(
        (Vector2){1720.74f, 1596.39f},
        (Vector2){1760.74f, 1583.39f},
        (Vector2){1760.74f, 1613.39f},
        (Vector2){1727.74f, 1628.39f}
    );

    add_isometric_obstacle(
        (Vector2){1762.74f, 1582.39f},
        (Vector2){2133.74f, 1771.39f},
        (Vector2){2111.74f, 1786.39f},
        (Vector2){1757.74f, 1612.39f}
    );

    add_isometric_obstacle(
        (Vector2){2109.74f, 1820.39f},
        (Vector2){2110.74f, 1799.39f},
        (Vector2){2233.74f, 1849.39f},
        (Vector2){2208.74f, 1867.39f}
    );

    add_isometric_obstacle(
        (Vector2){2208.74f, 1908.39f},
        (Vector2){2205.74f, 1931.39f},
        (Vector2){2397.74f, 2028.39f},
        (Vector2){2397.74f, 2003.39f}
    );

    add_isometric_obstacle(
        (Vector2){2395.53f, 1999.9f},
        (Vector2){2938.53f, 1732.9f},
        (Vector2){2940.53f, 1757.9f},
        (Vector2){2400.53f, 2028.9f}
    );
}
