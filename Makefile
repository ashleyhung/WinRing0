Q           :=@
CC          :=g++
SRCS        :=$(wildcard *.cpp)
TARGET      :=GetWinInfo.exe
OBJECTS     :=
CFLAGS      :=-Iinclude/
LIBS        :=-m32
LIBS        +=-L./bin/WinRing0.dll
LDFLAGS     :=$(LIBS) -lm
OUTPATH		:=./bin
.PHONY: all
all: clean $(TARGET)

$(TARGET):
	$(Q)echo "  Building '$@' ..."
	$(Q)$(CC) -g $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)
	$(Q)mv $(TARGET) $(OUTPATH)

.PHONY: clean
clean:
	$(Q)echo "  Cleaning '$(TARGET)' ..."
	$(Q)$(RM) $(TARGET)