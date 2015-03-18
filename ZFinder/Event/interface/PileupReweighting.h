#ifndef ZFINDER_PILEUPREWEIGHTING_H_
#define ZFINDER_PILEUPREWEIGHTING_H_

// Standard Library
#include <vector>  // std::vector

namespace zf {
    // These are set up using:
    //   ./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json
    //   --inputLumiJSON
    //   ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt
    //   --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60
    //   output_60_v2.root
    //
    // And then:
    //
    //   ./histo_to_vector.py output_60.root
    //
    // This vector will be normalized by the constructor when it is passed.
    //
    static std::vector<float> RUN_2012_ABCD_TRUE_PILEUP = {
        13531.4967008,
        21824.9441986,
        41057.5926428,
        249375.424445,
        556933.109802,
        3644879.97715,
        20244864.34,
        60932249.0248,
        143044498.481,
        276249609.039,
        471757322.888,
        705493863.615,
        886930974.382,
        995894421.048,
        1069961493.54,
        1123834132.33,
        1151439768.42,
        1156183443.53,
        1150469717.27,
        1135863770.57,
        1111746247.48,
        1082392424.66,
        1050251516.92,
        1009505749.56,
        950040157.712,
        865895109.52,
        760206134.284,
        642339156.417,
        522345297.678,
        408582931.435,
        307364847.321,
        222429517.257,
        154750882.488,
        103333020.901,
        66160904.6509,
        40741545.9568,
        24388098.8371,
        14502583.4203,
        8860066.05763,
        5779365.85104,
        4131306.24449,
        3232596.36387,
        2705532.22673,
        2356467.72202,
        2092450.45756,
        1871057.34766,
        1673666.88822,
        1492423.53712,
        1324090.49898,
        1167532.78377,
        1022420.05856,
        888684.604558,
        766328.823227,
        655326.235933,
        555551.275229,
        466745.849876,
        388510.920483,
        320314.843904,
        261512.507148,
        211370.690115
    };

    // Using Run2012A.json
    static std::vector<float> RUN_2012_A_TRUE_PILEUP = {
        7.19555784861e-06,
        17.3419047226,
        3014.29139373,
        181833.624466,
        284606.950266,
        142648.958816,
        170442.404782,
        76081.815466,
        648276.744942,
        5882875.77458,
        22637443.7936,
        41680907.6234,
        55616196.05,
        67441923.8693,
        77976188.2598,
        84130487.6303,
        84943955.4945,
        80704698.3259,
        73306303.9265,
        65185507.4471,
        56750325.8169,
        47557348.9832,
        38053413.4988,
        28749677.7459,
        19962392.6725,
        12459626.22,
        6992929.57977,
        3598500.21228,
        1742428.85193,
        811374.580076,
        367600.612527,
        161925.676576,
        68560.8008363,
        27441.1233501,
        10214.839084,
        3490.52389844,
        1084.31700013,
        304.038149501,
        76.5462318515,
        17.2374774704,
        3.46238165491,
        0.619118911056,
        0.0984166254313,
        0.013894005395,
        0.00174074103808,
        0.000193439875358,
        1.90576404156e-05,
        1.66397500834e-06,
        1.28714361647e-07,
        8.8201189008e-09,
        5.38538129091e-10,
        2.26206442466e-11,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    // Using Run2012B.json
    static std::vector<float> RUN_2012_B_TRUE_PILEUP = {
        0.0320271608068,
        0.307045061935,
        3.48828587364,
        706.217623009,
        99400.4502206,
        2250266.38009,
        11731544.6928,
        29656306.6932,
        59126286.7728,
        96871819.8005,
        138048166.651,
        181746369.092,
        218640540.258,
        243404744.312,
        261796798.71,
        274517099.873,
        274326477.515,
        264565687.018,
        252078544.835,
        240419217.661,
        232068532.465,
        227923230.619,
        226833028.178,
        225535297.824,
        218613204.682,
        201291848.703,
        172622399.151,
        135855768.904,
        96808802.8187,
        61703729.5429,
        34864192.711,
        17378463.4137,
        7632229.3618,
        2958151.83313,
        1016768.29947,
        312826.251041,
        87451.686349,
        22659.2815458,
        5554.30087058,
        1306.43665038,
        295.82729434,
        64.1100204858,
        13.1837771721,
        2.55881054247,
        0.468841022073,
        0.0815401504135,
        0.013574755067,
        0.00218084356095,
        0.000339650981162,
        5.12501610371e-05,
        7.45435651989e-06,
        1.03759047021e-06,
        1.37176602224e-07,
        1.71126915479e-08,
        1.9984919275e-09,
        2.14420703415e-10,
        1.35308431126e-11,
        0.0,
        0.0,
        0.0
    };

    // Using Run2012C.json
    static std::vector<float> RUN_2012_C_TRUE_PILEUP = {
        11762.6987078,
        18155.616525,
        30682.8932101,
        52042.3305625,
        121678.930379,
        762791.609582,
        5703668.40804,
        24376255.3085,
        64438201.8578,
        121515562.139,
        202172856.919,
        293809531.854,
        355358095.143,
        383790211.86,
        402696249.498,
        420832997.283,
        432795081.17,
        436090248.442,
        435027043.925,
        428089384.732,
        414284759.393,
        395490072.544,
        374213764.568,
        349647643.993,
        318608983.947,
        279917552.683,
        236307260.398,
        192381221.884,
        151533870.793,
        115308357.444,
        84362728.2631,
        59032087.1558,
        39287325.0945,
        24708169.3569,
        14582940.4461,
        8032455.88265,
        4121525.18301,
        1978489.86916,
        899827.632399,
        396709.803729,
        175291.543545,
        80715.1084105,
        39988.0342702,
        21539.671243,
        12450.1726849,
        7534.46479082,
        4664.68194033,
        2906.32984416,
        1803.91774405,
        1108.92395723,
        672.953585938,
        402.4290517,
        236.915085971,
        137.236860762,
        78.2001485682,
        43.8274676967,
        24.1580688196,
        13.0962474441,
        6.98233556026,
        3.66124064354
    };

    // Using Run2012D.json
    static std::vector<float> RUN_2012_D_TRUE_PILEUP = {
        1768.76595861,
        3651.67872384,
        7356.9197531,
        14793.2517935,
        51246.7789356,
        489173.028661,
        2639208.83441,
        6823605.2077,
        18831733.1057,
        51979351.3247,
        108898855.524,
        188257055.045,
        257316142.931,
        301257541.006,
        327492257.072,
        344353547.544,
        359374254.237,
        374822809.746,
        390057824.584,
        402169660.734,
        408642629.8,
        411421772.511,
        411151310.68,
        405573129.997,
        392855576.41,
        372226081.914,
        344283545.154,
        310503665.417,
        272260195.214,
        230759469.868,
        187770325.734,
        145857041.011,
        107762767.231,
        75639258.5876,
        50550981.0663,
        32392773.2993,
        20178037.6508,
        12501130.2315,
        7954607.57813,
        5381332.37318,
        3955715.41127,
        3151816.52632,
        2665530.91027,
        2334925.47808,
        2079999.8143,
        1863522.80113,
        1669002.19268,
        1489517.20509,
        1322286.58089,
        1166423.85976,
        1021747.10496,
        888282.175506,
        766091.908141,
        655188.999072,
        555473.07508,
        466702.022408,
        388486.762414,
        320301.747657,
        261505.524812,
        211367.028875
    };

    // These are set up using:
    //   ./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json
    //   --inputLumiJSON
    //   ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt
    //   --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60
    //   output_2012ABCD_p5_v2.root
    //
    static std::vector<float> RUN_2012_ABCD_TRUE_PILEUP_PLUS = {
        12736.0129055,
        19974.4370872,
        34759.7421855,
        168674.482902,
        485191.547934,
        1859550.01915,
        12381252.0053,
        40478103.0217,
        98760006.0199,
        199124070.687,
        347748203.471,
        550648457.379,
        751600647.598,
        888180737.551,
        972565014.373,
        1034530740.14,
        1078335887.23,
        1098604052.73,
        1100887785.47,
        1095039822.67,
        1081362064.44,
        1059450564.58,
        1032927002.45,
        1004162380.18,
        968950219.143,
        918936629.468,
        847994745.426,
        756927263.487,
        652564292.479,
        543408852.478,
        436933599.34,
        339091227.543,
        254038856.881,
        183727726.299,
        128143069.07,
        86059416.5821,
        55649975.4934,
        34793482.9385,
        21270525.8678,
        12982152.5054,
        8154379.71045,
        5449196.4781,
        3959053.69952,
        3123660.2844,
        2624405.01658,
        2292095.70134,
        2042190.00153,
        1834401.23636,
        1650224.08695,
        1481438.14174,
        1324500.37533,
        1177984.80398,
        1041436.87799,
        914731.142888,
        797851.974894,
        690801.688494,
        593533.049146,
        505908.572586,
        427682.819978,
        358500.347061
    };

    // Using Run2012A.json
    static std::vector<float> RUN_2012_A_TRUE_PILEUP_PLUS = {
        2.99489150074e-06,
        12.7738450121,
        1396.29471533,
        110244.622747,
        326543.432191,
        108506.458012,
        188495.007288,
        91380.6807722,
        207331.740158,
        2237720.12637,
        12250489.481,
        30383151.4909,
        45537172.6831,
        57064549.1218,
        67604873.4383,
        76303971.2181,
        80737450.6791,
        80556739.7346,
        76137145.4404,
        69269409.0721,
        61896826.8316,
        54250790.5944,
        45937769.9428,
        37312909.6256,
        28833962.9224,
        20729417.2562,
        13562682.6161,
        8038952.54054,
        4374677.55519,
        2234249.83357,
        1093751.54852,
        520578.064065,
        241976.152908,
        109237.350588,
        47292.3058773,
        19345.8183077,
        7379.01089975,
        2597.20085832,
        837.004423797,
        245.552681019,
        65.2941751069,
        15.6863628545,
        3.39679797061,
        0.661886285862,
        0.115914392825,
        0.0182284915693,
        0.00257241166758,
        0.000325603258866,
        3.69506225257e-05,
        3.75833918254e-06,
        3.42513293251e-07,
        2.79661752434e-08,
        2.05001859821e-09,
        1.29259170034e-10,
        5.92944582323e-12,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    // Using Run2012B.json
    static std::vector<float> RUN_2012_B_TRUE_PILEUP_PLUS = {
        0.0283869835638,
        0.244264005864,
        2.25883245243,
        226.479157127,
        36253.1511573,
        1090103.61996,
        7508972.30264,
        20973044.1811,
        43627950.0522,
        75438750.5514,
        111364476.778,
        150467374.036,
        188602514.17,
        217965449.287,
        237519632.787,
        253404455.978,
        262561580.202,
        260169818.371,
        250603797.271,
        239221811.033,
        228723767.978,
        221143167.264,
        217220346.526,
        216079223.774,
        215238271.787,
        210373173.742,
        197157800.502,
        173923666.264,
        142476705.976,
        107109091.968,
        73071208.1008,
        44825703.1997,
        24582269.4222,
        12019670.9852,
        5240353.5887,
        2042082.47167,
        715078.314776,
        227142.868667,
        66385.6673086,
        18171.6901024,
        4740.32347885,
        1192.02675013,
        289.597228138,
        67.6177550641,
        15.0603066512,
        3.18361974797,
        0.638451989148,
        0.121958586978,
        0.0223460118772,
        0.00395653077543,
        0.000680478634251,
        0.000113815968506,
        1.8457956663e-05,
        2.88579542149e-06,
        4.32153842979e-07,
        6.17218637265e-08,
        8.23930107474e-09,
        1.06308684078e-09,
        1.07545639061e-10,
        0.0
    };

    // Using Run2012C.json
    static std::vector<float> RUN_2012_C_TRUE_PILEUP_PLUS = {
        11084.0569567,
        16662.6778945,
        26918.1696688,
        45775.8430239,
        89713.0501333,
        405624.578683,
        3045641.99274,
        14604192.2,
        43546795.5068,
        89071735.8746,
        150759422.442,
        233287948.294,
        308496673.178,
        350629220.673,
        371276388.49,
        388186893.121,
        404020068.181,
        413191936.141,
        415407947.433,
        414081541.533,
        407490319.181,
        394955576.982,
        378039234.621,
        358928498.14,
        337269050.283,
        310433971.58,
        276879353.171,
        238150910.179,
        197939706.948,
        159571146.315,
        124811890.164,
        94405945.8179,
        68741083.4675,
        47963470.1719,
        31905091.8443,
        20114876.329,
        11949395.2858,
        6661192.11649,
        3482696.51965,
        1716255.2851,
        806893.428861,
        369507.686096,
        169719.84889,
        80879.7226669,
        41137.9317341,
        22581.1096074,
        13255.1078271,
        8151.14631346,
        5141.88583045,
        3274.74748074,
        2083.96155145,
        1316.9144655,
        823.415451484,
        508.377122778,
        309.570468894,
        185.807375723,
        109.886940726,
        64.0221311984,
        36.7431303261,
        20.7714672777
    };

    // Using Run2012D.json
    static std::vector<float> RUN_2012_D_TRUE_PILEUP_PLUS = {
        1651.92755883,
        3298.74108362,
        6443.01896887,
        12427.5379748,
        32681.9144519,
        255315.362495,
        1638142.7026,
        4809485.95984,
        11377928.7207,
        32375864.1347,
        73373814.7701,
        136509983.558,
        208964287.567,
        262521518.468,
        296164119.658,
        316635419.823,
        331016788.168,
        344685558.483,
        358738895.324,
        372467061.036,
        383251150.446,
        389101029.745,
        391729651.363,
        391841748.637,
        387608934.15,
        377400066.89,
        360394909.137,
        336813734.504,
        307773201.999,
        274494364.362,
        237956749.527,
        199339000.461,
        160473527.839,
        123635347.792,
        90950331.3309,
        63883111.9631,
        42978122.8819,
        27902550.7525,
        17720606.6764,
        11247479.9775,
        7342680.66394,
        5078481.07889,
        3789040.85661,
        3042712.2821,
        2583251.90863,
        2269511.38988,
        2028934.25268,
        1826249.96776,
        1645082.17873,
        1478163.3903,
        1322416.4131,
        1176667.88941,
        1040613.46252,
        914222.765762,
        797542.404424,
        690615.881118,
        593423.162205,
        505844.550455,
        427646.076848,
        358479.575594
    };

    // These are set up using:
    //   ./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json
    //   --inputLumiJSON
    //   ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt
    //   --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60
    //   output_2012ABCD_m5_v2.root
    //
    static std::vector<float> RUN_2012_ABCD_TRUE_PILEUP_MINUS = {
        14431.409269,
        24008.8535748,
        50968.8202866,
        348053.97814,
        756611.694899,
        7023214.37289,
        32472179.035,
        92370857.4761,
        206557756.573,
        385002813.027,
        633725218.735,
        868260345.539,
        1014975759.97,
        1105770329.09,
        1171665253.47,
        1208753915.48,
        1217160594.63,
        1211754724.77,
        1196140694.1,
        1169468471.24,
        1136809569.16,
        1100578378.75,
        1052783610.11,
        981328657.411,
        881134493.732,
        758614261.491,
        626028854.267,
        495048826.443,
        374917916.145,
        271970536.758,
        188961392.926,
        125550722.396,
        79634691.6454,
        48300392.0857,
        28286405.4052,
        16355858.1636,
        9690338.89551,
        6152181.37539,
        4319401.04245,
        3349894.72497,
        2792486.14071,
        2424726.51653,
        2144500.16626,
        1907391.28729,
        1694877.27225,
        1499559.89193,
        1318607.66679,
        1151162.2014,
        997009.150894,
        856125.268681,
        728504.496947,
        614043.332888,
        512478.304249,
        423364.848687,
        346083.311963,
        279862.282524,
        223811.972049,
        176962.160864,
        138300.667155,
        106809.446985
    };

    // Using Run2012A.json
    static std::vector<float> RUN_2012_A_TRUE_PILEUP_MINUS = {
        1.86796175114e-05,
        29.5051061209,
        7060.47222706,
        266887.101958,
        214691.558476,
        185728.374415,
        137827.211832,
        146650.711578,
        2049327.25651,
        13648050.2745,
        35561772.1743,
        53203211.2276,
        66791070.1303,
        79195791.7575,
        87506876.2142,
        89652801.0482,
        85776543.3134,
        77827277.9328,
        68841814.062,
        59488558.1911,
        49271071.5128,
        38751437.3378,
        28504064.201,
        18989943.6868,
        11221751.2749,
        5923624.1837,
        2867736.58609,
        1311237.19698,
        578258.511115,
        247665.709678,
        102218.655204,
        39945.0746721,
        14492.187071,
        4801.34854751,
        1434.83057702,
        383.333780011,
        90.9683726566,
        19.0862090846,
        3.52882635828,
        0.573615395961,
        0.0818456680728,
        0.0102391771551,
        0.00112220428958,
        0.000107682310186,
        9.04204105005e-06,
        6.64136006109e-07,
        4.26542881088e-08,
        2.39749321784e-09,
        1.15444931392e-10,
        3.20795612296e-12,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    // Using Run2012B.json
    static std::vector<float> RUN_2012_B_TRUE_PILEUP_MINUS = {
        0.0365339640383,
        0.394646033566,
        6.25223055577,
        2333.83775275,
        268382.389804,
        4371353.67378,
        17704571.6305,
        42073007.6176,
        79404044.3042,
        123127008.207,
        170957923.692,
        216184638.417,
        248516242.011,
        270239492.917,
        286802722.205,
        289750561.416,
        280109166.328,
        266391369.985,
        253377155.446,
        244133277.419,
        239754694.101,
        238707684.578,
        236659251.518,
        226853270.731,
        204246982.713,
        169123628.148,
        126688815.958,
        84548203.1173,
        49628584.0057,
        25414072.027,
        11315137.2738,
        4382369.25471,
        1482565.51171,
        442052.474786,
        117991.959407,
        28822.5805244,
        6600.65262286,
        1442.44035157,
        302.203669661,
        60.300117725,
        11.3473837869,
        2.0024291418,
        0.331769808209,
        0.0519851485835,
        0.00778109752609,
        0.00112203630215,
        0.000156383919985,
        2.10030003919e-05,
        2.69820672829e-06,
        3.28614553213e-07,
        3.77438810461e-08,
        4.01483957191e-09,
        3.62045393665e-10,
        2.02660110915e-11,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };

    // Using Run2012C.json
    static std::vector<float> RUN_2012_C_TRUE_PILEUP_MINUS = {
        12528.6792652,
        19904.1067382,
        35408.0295498,
        60679.2285239,
        182029.526221,
        1532619.56306,
        10576124.0719,
        39794381.1977,
        92618693.5788,
        166896313.412,
        267639107.587,
        353522143.225,
        395955663.315,
        418220536.539,
        438703207.7,
        454002333.374,
        458879335.498,
        458166568.399,
        450874925.587,
        435598245.429,
        414604584.795,
        390755314.893,
        362598072.583,
        326380196.93,
        281666315.259,
        232742827.919,
        185041813.565,
        141821033.776,
        104392026.96,
        73355760.4482,
        48902461.1746,
        30711027.7439,
        18023908.0484,
        9814206.2508,
        4940629.60159,
        2307014.01228,
        1012265.24702,
        428041.700095,
        181215.389408,
        80363.8154013,
        38706.7345103,
        20436.2364232,
        11614.0594065,
        6900.67803886,
        4180.25333834,
        2538.97427559,
        1531.14386188,
        911.880738008,
        534.772759936,
        308.351136014,
        174.671187526,
        97.1675879711,
        53.0715127873,
        28.4579873409,
        14.9808549309,
        7.74208339385,
        3.92801173609,
        1.95653961064,
        0.956782768892,
        0.459361398634
    };

    // Using Run2012D.json
    static std::vector<float> RUN_2012_D_TRUE_PILEUP_MINUS = {
        1902.6934512,
        4074.84708444,
        8494.06627912,
        18153.8099054,
        91508.2203982,
        933512.761642,
        4053656.1208,
        10356817.9493,
        32485691.4331,
        81331441.1331,
        159566415.282,
        245350352.67,
        303712784.517,
        338114507.879,
        358652447.347,
        375348219.644,
        392395549.492,
        409369508.449,
        423046799.006,
        430248390.197,
        433179218.755,
        432363941.944,
        425022221.811,
        409105246.063,
        383999444.486,
        350824181.24,
        311430488.158,
        267368352.352,
        220319046.669,
        172953038.573,
        128641575.822,
        90417380.3224,
        60113725.8982,
        38039332.0116,
        23226349.0137,
        14019638.237,
        8671382.0275,
        5722678.14873,
        4137879.92054,
        3269470.03583,
        2753767.97697,
        2404288.26744,
        2132885.77396,
        1900490.55716,
        1690697.01112,
        1497020.91653,
        1317076.52277,
        1150250.32064,
        996474.378131,
        855816.917544,
        728329.825759,
        613946.1653,
        512425.232736,
        423336.390699,
        346068.331109,
        279854.540441,
        223808.044037,
        176960.204324,
        138299.710372,
        106808.987623
    };

    // From:
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/Pileup_MC_Gen_Scenarios
    static std::vector<float> SUMMER12_53X_MC_TRUE_PILEUP = {
        2.560E-06,
        5.239E-06,
        1.420E-05,
        5.005E-05,
        1.001E-04,
        2.705E-04,
        1.999E-03,
        6.097E-03,
        1.046E-02,
        1.383E-02,
        1.685E-02,
        2.055E-02,
        2.572E-02,
        3.262E-02,
        4.121E-02,
        4.977E-02,
        5.539E-02,
        5.725E-02,
        5.607E-02,
        5.312E-02,
        5.008E-02,
        4.763E-02,
        4.558E-02,
        4.363E-02,
        4.159E-02,
        3.933E-02,
        3.681E-02,
        3.406E-02,
        3.116E-02,
        2.818E-02,
        2.519E-02,
        2.226E-02,
        1.946E-02,
        1.682E-02,
        1.437E-02,
        1.215E-02,
        1.016E-02,
        8.400E-03,
        6.873E-03,
        5.564E-03,
        4.457E-03,
        3.533E-03,
        2.772E-03,
        2.154E-03,
        1.656E-03,
        1.261E-03,
        9.513E-04,
        7.107E-04,
        5.259E-04,
        3.856E-04,
        2.801E-04,
        2.017E-04,
        1.439E-04,
        1.017E-04,
        7.126E-05,
        4.948E-05,
        3.405E-05,
        2.322E-05,
        1.570E-05,
        5.005E-06
    };


    // From:
    // Rajdeep Mohan Chatterjee
    static std::vector<float> SUMMER12_53X_MC_TRUE_PILEUP_RAJDEEP = {
        2.29976e-06,
        5.65084e-06,
        1.17288e-05,
        5.09889e-05,
        9.65899e-05,
        0.000268743,
        0.00199044,
        0.00607028,
        0.0104655,
        0.0138727,
        0.0168198,
        0.0205214,
        0.0257178,
        0.0326346,
        0.041125,
        0.0497841,
        0.0553796,
        0.0572892,
        0.0560547,
        0.0531132,
        0.050097,
        0.047731,
        0.04558,
        0.0436234,
        0.0415995,
        0.0393506,
        0.036783,
        0.0340822,
        0.0311496,
        0.0281679,
        0.025192,
        0.0222683,
        0.0194782,
        0.0167733,
        0.0143733,
        0.0121372,
        0.0101492,
        0.00839675,
        0.00688824,
        0.00555973,
        0.00445338,
        0.00354403,
        0.00277279,
        0.0021487,
        0.00165284,
        0.00126874,
        0.000955583,
        0.000703923,
        0.000527499,
        0.000384421,
        0.000279684,
        0.000203397,
        0.000146823,
        0.000100302,
        7.16211e-05,
        5.12189e-05,
        3.47921e-05,
        2.37861e-05,
        1.64926e-05,
        5.05947e-06
    };
}  // namespace zf
#endif  // ZFINDER_PILEUPREWEIGHTING_H_
