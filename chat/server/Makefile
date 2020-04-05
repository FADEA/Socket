cc = g++
prom = server
deps = calc.h
obj = server.o

$(prom):$(obj)
	$(cc) -o $(prom) $(obj)

%.o:%.cc $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)

