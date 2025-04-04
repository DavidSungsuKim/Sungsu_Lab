import math

# V_measured --> T in Celsius
# 0.218 --> 254.9
# 4.159 --> 0.156

def calculate_temperature(V_measured, printLog = True):
    # Constants
    R0 = 10000   # Resistance at T0 (25 degrees Celsius)
    Beta = 3950  # Beta value for the thermistor
    T0 = 298.15  # Reference temperature in Kelvin (25 degrees Celsius)

    # Calculate Rt using the measured voltage
    Rt = -7480 * V_measured / (13 * V_measured - 55)

    # Calculate temperature in Kelvin
    num = 1
    den = 1 / T0 + 1 / Beta * math.log(Rt / R0)
    T = num / den

    temp = T - 273.15
    if printLog == True:
      print(f"V={V_measured:.3f}[V], Temperature = {temp:.3f}[C]")
    
    return T - 273.15  # Return temperature in Kelvin and Celsius

if __name__ == "__main__":
   # Example usage
   calculate_temperature(3.88)
   calculate_temperature(3.84)


