cc = g++
prom = client
deps = config.h
obj = client.o

$(prom):$(obj)
	$(cc) -o $(prom) $(obj) -pthread

%.o:%.cc $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)