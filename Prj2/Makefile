SUB_DIR := ./StoogeFarmers ./Party

all:
	@for n in $(SUB_DIR); do $(MAKE) -C $$n || exit 1; done

clean:
	@for n in $(SUB_DIR); do $(MAKE) -C $$n clean || exit 1; done
