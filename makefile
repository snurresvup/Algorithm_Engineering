CC = g++
CFLAGS = -O3 -std=c++11
EXEC = main.o
OUTDIR = out
PLOT = graph.png

PLOTS_DIR = plot_scripts
TEMPLATES = $(wildcard *.tracetemplate)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
PLOT_SCRIPTS = $(SOURCES:%.cpp=$(PLOTS_DIR)/%_plot_script)
DATA_FILES = $(SOURCES:%.cpp=$(OUTDIR)/%.dat)

define \n


endef

$(OUTDIR) :
	mkdir $@

%.o : %.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $@

$(OUTDIR)/%.dat : %.o $(OUTDIR)
	./$< >> $@

plot_all : $(PLOT_SCRIPTS) $(DATA_FILES)
	gnuplot $<

trace_and_plot_all : $(TEMPLATES) plot_all $(OBJECTS)
	$(foreach template, $(TEMPLATES), $(foreach object, $(OBJECTS), instruments -t $(template) -D $(OUTDIR)/$(object:.o=-$(template:.tracetemplate=.trace)) $(object)${\n}))
	$(info Traces and plots can be found in the out directory)

trace_bst_multiple : binary_search_BST_BFS.o $(TEMPLATES)
	$(foreach num, 7 49 2401 32768, \
		$(foreach template, $(TEMPLATES), instruments -t $(template) -D $(OUTDIR)/$(<:.o=-$(num)$(template:.tracetemplate=.trace)) $< $(num) ${\n}))


clean :
	rm -f $(OBJECTS)
	rm -rf $(OUTDIR)

