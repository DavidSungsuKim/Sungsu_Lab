import math

# V_measured --> T in Celsius
# 0.218 --> 254.9
# 4.159 --> 0.156

V_measured = 3.86
Rt = -7480 * V_measured / ( 13 * V_measured - 55 )
Beta = 3950
R0 = 10000
T0 = 298.15 # Kelvin (25 degrees Celsius)

print("Rt={}, R0={}".format(Rt, R0))

num = 1
den = 1 / T0 + 1 / Beta * math.log( Rt / R0 )
T = num / den

print( "Temperature in Kelvin: ", T )
print( "Temperature in Celsius: ", T - 273.15 )

