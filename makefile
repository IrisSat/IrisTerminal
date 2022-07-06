CC=gcc
FLAGS = -g3 -O0



INC = -I./Libraries/libcsp/include  -I./Libraries/libserialport -Iinclude -Iinclude/utilities -Iinclude/commands -Iinclude/handlers
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

all: src/main.c src/drivers/usart_windows2.c src/csp_client.c src/commands/payloadCommands.c src/commands/powerCommands.c src/networkConfig.c src/commands/cdhCommands.c src/telemetry.c src/utilities/loggingUtilities.c src/utilities/settingsUtilities.c src/utilities/utilities.c src/commands/commsCommands.c src/handlers/telemetry_handlers.c
	$(CC) $(FLAGS) -o IrisTerminal src/main.c src/drivers/usart_windows2.c src/csp_client.c src/commands/payloadCommands.c src/commands/powerCommands.c src/networkConfig.c src/commands/cdhCommands.c src/telemetry.c src/utilities/loggingUtilities.c src/utilities/settingsUtilities.c src/utilities/utilities.c src/commands/commsCommands.c src/handlers/telemetry_handlers.c $(INC) $(LIBDIR) $(LIBS) 

clean:
	rm *.exe