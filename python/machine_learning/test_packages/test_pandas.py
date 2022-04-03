from IPython.display import display
import pandas as pd

print("version pandas: ", pd.__version__)

data = {'name' : ["banana", "apple", "strawberry"],
        'price': [100,200,300]
        }

data_pandas = pd.DataFrame(data)
display(data_pandas)