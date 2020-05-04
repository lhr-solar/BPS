
ifneq ($(TEST), none)
TEST_FILE := Test_$(TEST).c
# This line adds everything in Apps/Src/*.c except for main.c
SRC += \
$(filter-out ../../Apps/Src/main.c, $(wildcard ../../Apps/Src/*.c))	\
$(wildcard ../../Tests/$(TEST_FILE))
else
C_SOURCES += $(wildcard ../../Apps/Src/*.c)
endif
