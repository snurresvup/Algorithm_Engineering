CC = g++
CFLAGS = -O3 -std=c++11
EXEC = main.o
OUTDIR = out
PLOT = graph.png

PLOTS_DIR = plot_scripts
TEMPLATE_DIR = TraceTemplates
SOURCE_DIR = src
OBJECT_DIR = obj
TEMPLATES = $(wildcard $(TEMPLATE_DIR)/*.tracetemplate)
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)
PLOT_SCRIPTS = $(SOURCES:%.cpp=$(PLOTS_DIR)/%_plot_script)
DATA_FILES = $(SOURCES:%.cpp=$(OUTDIR)/%.dat)

define \n


endef

$(OUTDIR) :
	mkdir $@

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(OUTDIR)
	$(CC) $(CFLAGS) $< -o $@

$(OUTDIR)/%.dat : $(OBJECT_DIR)/%.o $(OUTDIR)
	./$< >> $@

plot_all : $(PLOT_SCRIPTS) $(DATA_FILES)
	gnuplot $<

time_dfsl : $(OBJECT_DIR)/binary_search_BST_DFSl.o
	$(foreach num, $(shell ruby -e '(0..25).each do |n| print "#{2**n} " end'), \
		./$< $(num) 0.5 1 ${\n})


trace_and_plot_all : $(TEMPLATES) plot_all $(OBJECTS)
	$(foreach template, $(TEMPLATES), $(foreach object, $(OBJECTS), instruments -t $(template) -D $(OUTDIR)/$(object:$(OBJECT_DIR)/%.o=%-$(template:$(TEMPLATE_DIR)/%.tracetemplate=%.trace)) $(object)${\n}))
	$(info Traces and plots can be found in the out directory)

trace_bst_multiple : $(OBJECT_DIR)/binary_search_BST_BFS.o $(TEMPLATES)
	$(foreach num, 7 49 2401 32768, \
		$(foreach template, $(TEMPLATES), instruments -t $(template) -D $(OUTDIR)/$(<:$(OBJECT_DIR)/%.o=%-$(num)$(template:$(TEMPLATE_DIR)/%.tracetemplate=%.trace)) $< $(num) ${\n}))

trace_bst_dfsl_multiple : $(OBJECT_DIR)/binary_search_BST_DFSl.o $(TEMPLATES)
	$(foreach num, 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9, \
		$(foreach template, $(TEMPLATES), \
			instruments -t $(template) -D $(OUTDIR)/dfsl_skew_$(num)_$(template:$(TEMPLATE_DIR)/%.tracetemplate=%.trace) $< 10000000 $(num) 1 ${\n} \
			./TraceUtil $(OUTDIR)/dfsl_skew_$(num)_$(template:$(TEMPLATE_DIR)/%.tracetemplate=%.trace) | ruby -pe 'print "$(num) "' >> $(OUTDIR)/$(template:$(TEMPLATE_DIR)/%.tracetemplate=%) ${\n})) \

test_trace_bst_dfsl_multiple : $(OBJECT_DIR)/binary_search_BST_DFSl.o $(TEMPLATES)
	$(foreach num, 0.3 0.4 0.5 0.6 0.7, \
		./$< 10000000 $(num) 1 ${\n})
test :
	for num in 0.1 2 3 4 5 ; do echo $$num ; done


clean :
	rm -f $(OBJECTS)
	rm -rf $(OUTDIR)

