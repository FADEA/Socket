cc = g++
prom = server
deps = config.h
obj = server.o

$(prom):$(obj)
	$(cc) -o $(prom) $(obj) -pthread

%.o:%.cc $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)

