ALL=c_ttyredirect isatty

all: $(ALL)

c_ttyredirect = -lutil

%: %.c
	$(CC) $< $($@) -o $@

clean:
	rm -rf $(ALL)
