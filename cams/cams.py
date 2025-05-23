import numpy as np
import scipy
import math
from cartesian import *



#cam1
mtx1 = np.array([
    [ 628.49639982, 0.0, 639.43835794],
    [ 0.0, 629.29462499, 361.07535495],
    [ 0.0, 0.0, 1.0]
])
dist1 = np.array( [9.84535386e-02, -3.80486334e-02, -4.49539881e-03,  2.85065003e-05, -4.77705039e-02] )

mtx2 = np.array([
    [621.86283081, 0.0, 638.01012662],
    [0.0, 621.48430252, 357.60141644],
    [ 0.0, 0.0, 1.0]
])
dist2 = np.array([0.1133566,  -0.10474354, -0.00360539, -0.00078946,  0.04916464] )

mtx3 = np.array([[616.27784461,   0.,         640.92196493],
 [  0.,         616.78715286, 370.50921222],
 [  0.,           0.,           1.        ]])
dist3 = np.array( [[ 0.09338165, -0.07603509,  0.00072092,  0.00256457,  0.03110497]])

mtx4 = np.array( [[610.70215529,   0.,         639.95135753],
 [  0.,         610.45072585, 362.20373558],
 [  0.,          0.,           1.        ]] )
dist4 = np.array( [[ 0.08772232, -0.03140344, -0.00100429,  0.00120651, -0.0021742 ]])

cam1 = np.array( 
    [[-0.9998137,   0.00447402, -0.01877604,  0.51669241],
 [ 0.01437869,  0.82159943, -0.56988387,  0.15782646],
 [ 0.01287671, -0.57004767, -0.82151071,  0.53272   ],
 [ 0. ,         0.,          0.,          1.        ]])

cam2 = np.array(
    [[-0.00650127,  0.82250282, -0.56872387,  0.91814014],
 [ 0.99968615, -0.00841433, -0.02359676, -0.33790907],
 [-0.02419383, -0.56869879, -0.82218997,  0.46424805],
 [ 0.,          0.,          0.,          1.        ]])

cam3 = np.array(
    [[ 0.99770759, -0.00437807,  0.06753071,  0.5785536 ],
 [-0.04452901, -0.79390589,  0.60640795, -0.75239822],
 [ 0.05095813, -0.60802489, -0.79228088,  0.52932356],
 [ 0.,          0.,          0.,          1.        ]]
)
cam4 = np.array(
[[ 0.38207155, -0.74728205,  0.54368269,  0.17398149],
 [-0.85244271, -0.05779284,  0.51961661, -0.58850281],
 [-0.3568792,  -0.66198907, -0.65909613,  0.54060849],
 [ 0.,          0.,          0.,          1.        ]]
)


calib_E_1_2 = np.array([
    [ 0.01899019,  0.71493924, -0.69892865, 0.47955101],
    [-0.73963041,  0.48041605,  0.47132503, -0.34617139],
    [ 0.6727453,   0.50799833,  0.53791399, 0.25750319],
    [ 0.,          0.,          0.,          1.       ]
    ])

calib_E_2_3 = np.array([
    [-0.02778379,  0.68829588, -0.72489781, 0.27993001],
    [-0.73088782,  0.48072489,  0.48446525, -0.25097295],
    [ 0.68193186,  0.54327926,  0.48971072, 0.35349544],
    [ 0.,          0.,          0.,          1.       ]
    ])

calib_E_3_4 = np.array([
    [ 0.38991166,  0.87171213, -0.29679432, 0.26768416],
    [-0.73895082,  0.48851201,  0.46401261, -0.28147912],
    [ 0.54947301,  0.03839248,  0.83462891, 0.18662761],
    [ 0.,          0.,          0.,          1.       ]
    ])

calib_E_1_4 = np.array([
    [-0.40977408, -0.35553952,  0.84004574, -0.53791122],
    [ 0.68881794,  0.48312434,  0.54048193, -0.19086703],
    [-0.59800923,  0.80011407,  0.04693017, 0.59195299],
    [ 0.,          0.,          0.,          1.       ]
    ])

# E_from_to
E_2_1 = Fi(cam1) @ cam2
# print("E_1_0:\n", E_2_1)
E_1_2 = Fi(E_2_1)
print("E_1_2 - calib_E_1_2:\n", E_1_2 - calib_E_1_2)

E_3_2 = Fi(cam2) @ cam3
# print("E_1_0:\n", E_2_1)
E_2_3 = Fi(E_3_2)
print("E_2_3 - calib_E_2_3:\n", E_2_3 - calib_E_2_3)


E_4_3 = Fi(cam3) @ cam4
# print("E_1_0:\n", E_2_1)
E_3_4 = Fi(E_4_3)
print("E_3_4 - calib_E_3_4:\n", E_3_4 - calib_E_3_4)

E_4_1 = Fi(cam1) @ cam4
# print("E_1_0:\n", E_2_1)
E_1_4 = Fi(E_4_1)
print("E_1_4 - calib_E_1_4:\n", E_1_4 - calib_E_1_4)

print("\ncalib self test:")
test_E_1_4 = calib_E_3_4 @ calib_E_2_3 @ calib_E_1_2
print("test_E_1_4 - calib_E_1_4:\n", test_E_1_4 - calib_E_1_4)

calib_cam1 = cam1
calib_cam2 = cam1 @ Fi( calib_E_1_2)
# print("calib_cam2 - cam2:\n", calib_cam2 - cam2)

calib_cam3 = cam1 @ Fi( calib_E_1_2) @ Fi( calib_E_2_3)
# print("calib_cam3 - cam3:\n", calib_cam3 - cam3)

calib_cam4 = cam1 @ Fi( calib_E_1_2) @ Fi( calib_E_2_3) @ Fi( calib_E_3_4)
# print("calib_cam4 - cam4:\n", calib_cam4 - cam4)

# print("calib_cam1:\n", calib_cam1)
# print("calib_cam2:\n", calib_cam2)
# print("calib_cam3:\n", calib_cam3)
# print("calib_cam4:\n", calib_cam4)
# NSH
# cam1 cam2
# run1
# error:  0.0003127770240530697
# R:
#  [[ 0.01899019  0.71493924 -0.69892865]
#  [-0.73963041  0.48041605  0.47132503]
#  [ 0.6727453   0.50799833  0.53791399]]
# T:
#  [[ 0.47955101]
#  [-0.34617139]
#  [ 0.25750319]]

# cam2 & cam3
# error:  0.0006186194266184225
# R:
#  [[-0.02778379  0.68829588 -0.72489781]
#  [-0.73088782  0.48072489  0.48446525]
#  [ 0.68193186  0.54327926  0.48971072]]
# T:
#  [[ 0.27993001]
#  [-0.25097295]
#  [ 0.35349544]]


# cam3 & cam4
# error:  0.000571792689288192
# R:
#  [[ 0.38991166  0.87171213 -0.29679432]
#  [-0.73895082  0.48851201  0.46401261]
#  [ 0.54947301  0.03839248  0.83462891]]
# T:
#  [[ 0.26768416]
#  [-0.28147912]
#  [ 0.18662761]]

# cam1 & cam4
# error:  0.0014211170363042313
# R:
#  [[-0.40977408 -0.35553952  0.84004574]
#  [ 0.68881794  0.48312434  0.54048193]
#  [-0.59800923  0.80011407  0.04693017]]
# T:
#  [[-0.53791122]
#  [-0.19086703]
#  [ 0.59195299]]