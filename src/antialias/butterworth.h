#pragma once

#include <map>
#include <vector>
#include <tuple>

struct IIR {
    const double a[4];
    const double b[5];
};

// 12.5kHz, 4 pole Butterworth LPF
const std::map<int, IIR> COEFFICIENTS_12500Hz = {
    {
        11025,
        IIR{
            .a = {-0.000000, 0.486029, -0.000000, 0.017665},
            .b = {0.093981, 0.375923, 0.563885, 0.375923, 0.093981}
        },
    },
    {
        12000,
        IIR{
            .a = {-0.000000, 0.486029, -0.000000, 0.017665},
            .b = {0.093981, 0.375923, 0.563885, 0.375923, 0.093981}
        },
    },
    {
        22050,
        IIR{
            .a = {-0.000000, 0.486029, -0.000000, 0.017665},
            .b = {0.093981, 0.375923, 0.563885, 0.375923, 0.093981}
        },
    },
    {
        24000,
        IIR{
            .a = {-0.000000, 0.486029, -0.000000, 0.017665},
            .b = {0.093981, 0.375923, 0.563885, 0.375923, 0.093981}
        },
    },
    {
        44100,
        IIR{
            .a = {0.522771, 0.572532, 0.115179, 0.023070},
            .b = {0.139597, 0.558388, 0.837582, 0.558388, 0.139597}
        },
    },
    {
        48000,
        IIR{
            .a = {0.162720, 0.494399, 0.034292, 0.018177},
            .b = {0.106849, 0.427397, 0.641096, 0.427397, 0.106849}
        },
    },
    {
        88200,
        IIR{
            .a = {-1.701848, 1.415432, -0.555482, 0.088803},
            .b = {0.015432, 0.061726, 0.092589, 0.061726, 0.015432}
        },
    },
    {
        96000,
        IIR{
            .a = {-1.885259, 1.630530, -0.667897, 0.109585},
            .b = {0.011685, 0.046740, 0.070109, 0.046740, 0.011685}
        },
    },
    {
        176400,
        IIR{
            .a = {-2.840868, 3.148688, -1.593312, 0.308840},
            .b = {0.001459, 0.005837, 0.008756, 0.005837, 0.001459}
        },
    },
    {
        192000,
        IIR{
            .a = {-2.934464, 3.335403, -1.724001, 0.340343},
            .b = {0.001080, 0.004320, 0.006480, 0.004320, 0.001080}
        },
    },
    {
        352800,
        IIR{
            .a = {-3.418841, 4.419496, -2.556930, 0.558141},
            .b = {0.000117, 0.000466, 0.000700, 0.000466, 0.000117}
        },
    },
    {
        384000,
        IIR{
            .a = {-3.465980, 4.535945, -2.653948, 0.585341},
            .b = {0.000085, 0.000339, 0.000509, 0.000339, 0.000085}
        },
    },
    {
        705600,
        IIR{
            .a = {-3.709207, 5.169158, -3.207302, 0.747485},
            .b = {0.000008, 0.000033, 0.000050, 0.000033, 0.000008}
        },
    },
    {
        768000,
        IIR{
            .a = {-3.732824, 5.233586, -3.266057, 0.765391},
            .b = {0.000006, 0.000024, 0.000036, 0.000024, 0.000006}
        },
    },
    {
        1411200,
        IIR{
            .a = {-3.854577, 5.574210, -3.584254, 0.864629},
            .b = {0.000001, 0.000002, 0.000003, 0.000002, 0.000001}
        },
    },
    {
        1536000,
        IIR{
            .a = {-3.866391, 5.608026, -3.616540, 0.874911},
            .b = {0.000000, 0.000002, 0.000002, 0.000002, 0.000000}
        },
    },
    {
        2822400,
        IIR{
            .a = {-3.927285, 5.784487, -3.787063, 0.929862},
            .b = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000}
        },
    },
    {
        3072000,
        IIR{
            .a = {-3.933193, 5.801801, -3.803980, 0.935373},
            .b = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000}
        },
    },
};

// 16kHz, 4 pole Butterworth LPF
const std::map<int, IIR> COEFFICIENTS_16kHz = {
    {
        11025,
        IIR{
            .a = {0.782095, 0.679979, 0.182676, 0.030119},
            .b = {0.167179, 0.668717, 1.003076, 0.668717, 0.167179}
        },
    },
    {
        12000,
        IIR{
            .a = {0.782095, 0.679979, 0.182676, 0.030119},
            .b = {0.167179, 0.668717, 1.003076, 0.668717, 0.167179}
        },
    },
    {
        22050,
        IIR{
            .a = {0.782095, 0.679979, 0.182676, 0.030119},
            .b = {0.167179, 0.668717, 1.003076, 0.668717, 0.167179}
        },
    },
    {
        24000,
        IIR{
            .a = {0.782095, 0.679979, 0.182676, 0.030119},
            .b = {0.167179, 0.668717, 1.003076, 0.668717, 0.167179}
        },
    },
    {
        44100,
        IIR{
            .a = {1.774009, 1.497258, 0.597776, 0.096508},
            .b = {0.310347, 1.241388, 1.862081, 1.241388, 0.310347}
        },
    },
    {
        48000,
        IIR{
            .a = {1.306605, 1.030454, 0.362369, 0.055764},
            .b = {0.234699, 0.938798, 1.408197, 0.938798, 0.234699}
        },
    },
    {
        88200,
        IIR{
            .a = {-1.074228, 0.852962, -0.274000, 0.042262},
            .b = {0.034187, 0.136749, 0.205124, 0.136749, 0.034187}
        },
    },
    {
        96000,
        IIR{
            .a = {-1.306605, 1.030454, -0.362369, 0.055764},
            .b = {0.026078, 0.104311, 0.156466, 0.104311, 0.026078}
        },
    },
    {
        176400,
        IIR{
            .a = {-2.519465, 2.560837, -1.206235, 0.220129},
            .b = {0.003454, 0.013817, 0.020725, 0.013817, 0.003454}
        },
    },
    {
        192000,
        IIR{
            .a = {-2.638628, 2.769310, -1.339281, 0.249822},
            .b = {0.002576, 0.010306, 0.015459, 0.010306, 0.002576}
        },
    },
    {
        352800,
        IIR{
            .a = {-3.256569, 4.033768, -2.246044, 0.473506},
            .b = {0.000291, 0.001165, 0.001748, 0.001165, 0.000291}
        },
    },
    {
        384000,
        IIR{
            .a = {-3.316808, 4.174246, -2.357403, 0.503375},
            .b = {0.000213, 0.000853, 0.001279, 0.000853, 0.000213}
        },
    },
    {
        705600,
        IIR{
            .a = {-3.627844, 4.951225, -3.011924, 0.688888},
            .b = {0.000022, 0.000086, 0.000129, 0.000086, 0.000022}
        },
    },
    {
        768000,
        IIR{
            .a = {-3.658060, 5.031434, -3.083228, 0.710104},
            .b = {0.000016, 0.000062, 0.000093, 0.000062, 0.000016}
        },
    },
    {
        1411200,
        IIR{
            .a = {-3.813865, 5.458724, -3.474943, 0.830108},
            .b = {0.000001, 0.000006, 0.000009, 0.000006, 0.000001}
        },
    },
    {
        1536000,
        IIR{
            .a = {-3.828986, 5.501430, -3.515194, 0.842767},
            .b = {0.000001, 0.000004, 0.000006, 0.000004, 0.000001}
        },
    },
    {
        2822400,
        IIR{
            .a = {-3.906926, 5.725084, -3.729275, 0.911118},
            .b = {0.000000, 0.000000, 0.000001, 0.000000, 0.000000}
        },
    },
    {
        3072000,
        IIR{
            .a = {-3.914488, 5.747100, -3.750647, 0.918036},
            .b = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000}
        },
    },
};


// 12.5kHz, 4 pole Butterworth LPF transfer function (44.1kHz)
const std::vector<std::tuple<double, double>> TF_12500Hz = {
    {     0.00000, 1.00000 },{   172.26562, 1.00000 },{   344.53125, 1.00000 },{   516.79688, 1.00000 },
    {   689.06250, 1.00000 },{   861.32812, 1.00000 },{  1033.59375, 1.00000 },{  1205.85938, 1.00000 },
    {  1378.12500, 1.00000 },{  1550.39062, 1.00000 },{  1722.65625, 1.00000 },{  1894.92187, 1.00000 },
    {  2067.18750, 1.00000 },{  2239.45313, 1.00000 },{  2411.71875, 1.00000 },{  2583.98438, 1.00000 },
    {  2756.25000, 1.00000 },{  2928.51562, 1.00000 },{  3100.78125, 1.00000 },{  3273.04687, 1.00000 },
    {  3445.31250, 1.00000 },{  3617.57812, 1.00000 },{  3789.84375, 1.00000 },{  3962.10937, 1.00000 },
    {  4134.37500, 0.99999 },{  4306.64062, 0.99999 },{  4478.90625, 0.99999 },{  4651.17188, 0.99998 },
    {  4823.43750, 0.99998 },{  4995.70312, 0.99997 },{  5167.96875, 0.99995 },{  5340.23438, 0.99994 },
    {  5512.50000, 0.99992 },{  5684.76563, 0.99990 },{  5857.03125, 0.99986 },{  6029.29688, 0.99982 },
    {  6201.56250, 0.99977 },{  6373.82813, 0.99970 },{  6546.09375, 0.99962 },{  6718.35938, 0.99952 },
    {  6890.62500, 0.99939 },{  7062.89062, 0.99923 },{  7235.15625, 0.99903 },{  7407.42188, 0.99878 },
    {  7579.68750, 0.99847 },{  7751.95313, 0.99810 },{  7924.21875, 0.99763 },{  8096.48438, 0.99706 },
    {  8268.75000, 0.99637 },{  8441.01562, 0.99552 },{  8613.28125, 0.99448 },{  8785.54688, 0.99322 },
    {  8957.81250, 0.99169 },{  9130.07812, 0.98983 },{  9302.34375, 0.98759 },{  9474.60938, 0.98488 },
    {  9646.87500, 0.98161 },{  9819.14062, 0.97768 },{  9991.40625, 0.97296 },{ 10163.67188, 0.96732 },
    { 10335.93750, 0.96060 },{ 10508.20312, 0.95262 },{ 10680.46875, 0.94318 },{ 10852.73438, 0.93208 },
    { 11025.00000, 0.91908 },{ 11197.26562, 0.90398 },{ 11369.53125, 0.88656 },{ 11541.79688, 0.86662 },
    { 11714.06250, 0.84404 },{ 11886.32812, 0.81871 },{ 12058.59375, 0.79065 },{ 12230.85938, 0.75993 },
    { 12403.12500, 0.72674 },{ 12575.39062, 0.69139 },{ 12747.65625, 0.65426 },{ 12919.92188, 0.61581 },
    { 13092.18750, 0.57655 },{ 13264.45312, 0.53701 },{ 13436.71875, 0.49770 },{ 13608.98438, 0.45908 },
    { 13781.25000, 0.42157 },{ 13953.51562, 0.38551 },{ 14125.78125, 0.35115 },{ 14298.04688, 0.31869 },
    { 14470.31250, 0.28824 },{ 14642.57812, 0.25986 },{ 14814.84375, 0.23355 },{ 14987.10937, 0.20929 },
    { 15159.37500, 0.18702 },{ 15331.64062, 0.16664 },{ 15503.90625, 0.14807 },{ 15676.17188, 0.13120 },
    { 15848.43750, 0.11592 },{ 16020.70312, 0.10212 },{ 16192.96875, 0.08968 },{ 16365.23438, 0.07851 },
    { 16537.50000, 0.06850 },{ 16709.76562, 0.05955 },{ 16882.03125, 0.05157 },{ 17054.29688, 0.04448 },
    { 17226.56250, 0.03820 },{ 17398.82812, 0.03265 },{ 17571.09375, 0.02777 },{ 17743.35938, 0.02349 },
    { 17915.62500, 0.01975 },{ 18087.89062, 0.01649 },{ 18260.15625, 0.01368 },{ 18432.42188, 0.01125 },
    { 18604.68750, 0.00918 },{ 18776.95312, 0.00742 },{ 18949.21875, 0.00593 },{ 19121.48438, 0.00468 },
    { 19293.75000, 0.00365 },{ 19466.01562, 0.00280 },{ 19638.28125, 0.00211 },{ 19810.54688, 0.00156 },
    { 19982.81250, 0.00113 },{ 20155.07812, 0.00079 },{ 20327.34375, 0.00054 },{ 20499.60938, 0.00035 },
    { 20671.87500, 0.00022 },{ 20844.14062, 0.00013 },{ 21016.40625, 0.00007 },{ 21188.67188, 0.00003 },
    { 21360.93750, 0.00001 },{ 21533.20312, 0.00000 },{ 21705.46875, 0.00000 },{ 21877.73438, 0.00000 },
    { 22050.00000, 0.00000 },{ 22222.26562, 0.00000 },{ 22394.53125, 0.00000 },{ 22566.79688, 0.00000 },
    { 22739.06250, 0.00001 },{ 22911.32812, 0.00003 },{ 23083.59375, 0.00007 },{ 23255.85938, 0.00013 },
    { 23428.12500, 0.00022 },{ 23600.39063, 0.00035 },{ 23772.65625, 0.00054 },{ 23944.92188, 0.00079 },
    { 24117.18750, 0.00113 },{ 24289.45312, 0.00156 },{ 24461.71875, 0.00211 },{ 24633.98438, 0.00280 },
    { 24806.25000, 0.00365 },{ 24978.51562, 0.00468 },{ 25150.78125, 0.00593 },{ 25323.04688, 0.00742 },
    { 25495.31250, 0.00918 },{ 25667.57812, 0.01125 },{ 25839.84375, 0.01368 },{ 26012.10938, 0.01649 },
    { 26184.37500, 0.01975 },{ 26356.64063, 0.02349 },{ 26528.90625, 0.02777 },{ 26701.17188, 0.03265 },
    { 26873.43750, 0.03820 },{ 27045.70312, 0.04448 },{ 27217.96875, 0.05157 },{ 27390.23438, 0.05955 },
    { 27562.50000, 0.06850 },{ 27734.76562, 0.07851 },{ 27907.03125, 0.08968 },{ 28079.29687, 0.10212 },
    { 28251.56250, 0.11592 },{ 28423.82812, 0.13120 },{ 28596.09375, 0.14807 },{ 28768.35938, 0.16664 },
    { 28940.62500, 0.18702 },{ 29112.89063, 0.20929 },{ 29285.15625, 0.23355 },{ 29457.42188, 0.25986 },
    { 29629.68750, 0.28824 },{ 29801.95312, 0.31869 },{ 29974.21875, 0.35115 },{ 30146.48438, 0.38551 },
    { 30318.75000, 0.42157 },{ 30491.01563, 0.45908 },{ 30663.28125, 0.49770 },{ 30835.54688, 0.53701 },
    { 31007.81250, 0.57655 },{ 31180.07812, 0.61581 },{ 31352.34375, 0.65426 },{ 31524.60938, 0.69139 },
    { 31696.87500, 0.72674 },{ 31869.14062, 0.75993 },{ 32041.40625, 0.79065 },{ 32213.67188, 0.81871 },
    { 32385.93750, 0.84404 },{ 32558.20312, 0.86662 },{ 32730.46875, 0.88656 },{ 32902.73438, 0.90398 },
    { 33075.00000, 0.91908 },{ 33247.26562, 0.93208 },{ 33419.53125, 0.94318 },{ 33591.79688, 0.95262 },
    { 33764.06250, 0.96060 },{ 33936.32812, 0.96732 },{ 34108.59375, 0.97296 },{ 34280.85938, 0.97768 },
    { 34453.12500, 0.98161 },{ 34625.39062, 0.98488 },{ 34797.65625, 0.98759 },{ 34969.92188, 0.98983 },
    { 35142.18750, 0.99169 },{ 35314.45312, 0.99322 },{ 35486.71875, 0.99448 },{ 35658.98438, 0.99552 },
    { 35831.25000, 0.99637 },{ 36003.51563, 0.99706 },{ 36175.78125, 0.99763 },{ 36348.04688, 0.99810 },
    { 36520.31250, 0.99847 },{ 36692.57812, 0.99878 },{ 36864.84375, 0.99903 },{ 37037.10938, 0.99923 },
    { 37209.37500, 0.99939 },{ 37381.64062, 0.99952 },{ 37553.90625, 0.99962 },{ 37726.17188, 0.99970 },
    { 37898.43750, 0.99977 },{ 38070.70312, 0.99982 },{ 38242.96875, 0.99986 },{ 38415.23438, 0.99990 },
    { 38587.50000, 0.99992 },{ 38759.76562, 0.99994 },{ 38932.03125, 0.99995 },{ 39104.29688, 0.99997 },
    { 39276.56250, 0.99998 },{ 39448.82812, 0.99998 },{ 39621.09375, 0.99999 },{ 39793.35938, 0.99999 },
    { 39965.62500, 0.99999 },{ 40137.89062, 1.00000 },{ 40310.15625, 1.00000 },{ 40482.42188, 1.00000 },
    { 40654.68750, 1.00000 },{ 40826.95312, 1.00000 },{ 40999.21875, 1.00000 },{ 41171.48438, 1.00000 },
    { 41343.75000, 1.00000 },{ 41516.01563, 1.00000 },{ 41688.28125, 1.00000 },{ 41860.54688, 1.00000 },
    { 42032.81250, 1.00000 },{ 42205.07812, 1.00000 },{ 42377.34375, 1.00000 },{ 42549.60937, 1.00000 },
    { 42721.87500, 1.00000 },{ 42894.14062, 1.00000 },{ 43066.40625, 1.00000 },{ 43238.67188, 1.00000 },
    { 43410.93750, 1.00000 },{ 43583.20312, 1.00000 },{ 43755.46875, 1.00000 },{ 43927.73438, 1.00000 },
};
extern IIR coefficients(const std::map<int, IIR>& table, float fs);
