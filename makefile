CC = g++
CFLAGS = -O3
EXEC = main.o
OUTPUT = data
PLOT = graph.png
OUTDIR = out/

$(OUTDIR) :
	mkdir $@

$(EXEC) : main.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $(OUTDIR)$@

$(OUTPUT) : $(EXEC) $(OUTDIR)
	./$< >> $(OUTDIR)$@

plot : $(OUTPUT)
	gnuplot plot_script

clean :
	rm -rf $(OUTDIR)
