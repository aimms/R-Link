df = expand.grid(doubles,cities,integers)
df[4] = values
df = df[,c(3,2,1,4)]