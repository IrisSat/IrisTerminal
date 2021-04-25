CC=gcc
FLAGS = -g3 -O0



INC = -I./Libraries/libcsp/include  -I./Libraries/libserialport -Iinclude
LIBDIR = -L./Libraries/libcsp -L./Libraries/libserialport
LIBS = -lcsp -lserialport -lsetupapi


# all: dir $(BUILDDIR)/$(EXECUTABLE)

# dir:
# 	mkdir -p $(BUILDDIR)

# $(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
# 	$(CC) $^ -o $@ $(INC) $(LIBDIR) $(LIBS) 

# $(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
# 	$(CC) $(FLAGS) $< -o $@ $(INC) 

# clean:
# 	rm -f $(BUILDDIR)/*o $(BUILDDIR)/$(EXECUTABLE)

all: src/main.c src/drivers/usart_windows2.c src/csp_client.c src/payloadCommands.c src/networkConfig.c src/cdhCommands.c
	$(CC) $(FLAGS) -o IrisTerminal src/main.c src/drivers/usart_windows2.c src/csp_client.c src/payloadCommands.c src/networkConfig.c src/cdhCommands.c $(INC) $(LIBDIR) $(LIBS) 

clean:
	rm *.exe