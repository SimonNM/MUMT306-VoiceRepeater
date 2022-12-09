# Project Name
TARGET = Recorder

# Sources
CPP_SOURCES = Recorder.cpp

# Library Locations
LIBDAISY_DIR = libDaisy/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
