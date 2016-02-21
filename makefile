CC = g++
CFLAGS = -O3
EXEC = main.o
OUTDIR = out/
OUTPUT = $(OUTDIR)data
PLOT = graph.png


$(OUTDIR) :
	mkdir $@

$(EXEC) : main.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT) : $(EXEC) $(OUTDIR)
	./$< >> $@

plot : $(OUTPUT)
	gnuplot plot_script

clean :
	rm -rf $(OUTDIR)
