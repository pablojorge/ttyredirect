import sys

sys.stdout.write("stdout is a tty: %s\n" % sys.stdout.isatty())
sys.stderr.write("stderr is a tty: %s\n" % sys.stderr.isatty())

for i in range(1,3):
    sys.stdout.write("stdout %i\n" % i)
    sys.stderr.write("stderr %i\n" % i)

