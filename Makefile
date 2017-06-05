.SUFFIX:

include config.mk

OBJECTS = $(patsubst %.cpp,build/%.o,$(SOURCES))
DEPS = $(patsubst %.cpp,build/%.deps,$(SOURCES))

.PHONY = all deps clean install
.DEFAULT_GOAL = all

all: $(BIN) $(LIB) $(HEADERS)

-include $(DEPS)

$(HEADERS): include/%: src/%
	@echo "[INST]" $(<:src/%=%)
	@install -d $(dir $<)
	@install -m 0644 $< $(dir $@)

$(OBJECTS): build/%.o: %.cpp
	@echo "[CXX] " $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEPS): build/%.deps: %.cpp
	@echo "[DEPS]" $@
	@mkdir -p $(dir $@)
	@$(DEPS_BIN) -std=c++11 -MM -MT build/$*.o $< > $@
	@$(DEPS_BIN) -std=c++11 -MM -MT build/$*.deps $< >> $@

$(BIN): bin/%:
	@echo "[LD]  " $@
	@$(CXX) $(LDFLAGS) -o $@ $^

$(LIB): lib/%:
	@echo "[AR]  " $@
	@$(AR) $(ARFLAGS) $@ $^

deps: $(DEPS)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(DEPS)
	@rm -f $(BIN)
	@rm -rf build/*
	@rm -rf include/*
	@rm -f $(LIB)

install: $(BIN) $(HEADERS) $(LIB)
ifneq ($(BIN),)
	@echo "[CP]  " binaries
	@mkdir -p $(PREFIX)/$(BIN_DIR)/
	@cp $(BIN) $(PREFIX)/$(BIN_DIR)/
endif
ifneq ($(wildcard include/*),)
	@echo "[CP]  " headers
	@mkdir -p $(PREFIX)/$(INCLUDE_DIR)/
	@cp -r include/* $(PREFIX)/$(INCLUDE_DIR)/
endif
ifneq ($(LIB),)
	@echo "[CP]  " libraries
	@mkdir -p $(PREFIX)/$(LIB_DIR)/
	@cp $(LIB) $(PREFIX)/$(LIB_DIR)/
endif

print-%:
	@echo $*=$($*)
