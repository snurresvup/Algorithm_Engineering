CC = g++
CFLAGS = -O3
EXEC = main.o
OUTDIR = out/
OUTPUT = $(OUTDIR)data
PLOT = graph.png


$(OUTDIR) :
	mkdir $@

$(EXEC) : binary_search.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT) : $(EXEC) $(OUTDIR)
	./$< >> $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $(OUTDIR)$@

plot-% : plot_script_% $(OUTDIR) $(OUTDIR)data_%
	gnuplot $@

run-% : %.o
	./$(OUTDIR)$<

data-% : %.o $(OUTDIR)
	./$(OUTDIR)$< >> $@


build : binary_search.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $(OUTDIR)/binary_search.o

plot : $(OUTPUT)
	gnuplot plot_script

clean :
	rm -rf $(OUTDIR)
	rm data*
