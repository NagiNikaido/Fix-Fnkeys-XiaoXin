#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/input.h>


void print_usage(FILE *stream, const char *program) {
    fprintf(stream, 
            "fix-fnkeys-xiaoxin - fix Lenovo XiaoXin Pro 2024's special fn-keys\n"
            "\n"
            "usage: %s [-h | -m]\n"
            "\n"
            "options:\n"
            "    -h        show this message and exit\n"
	    "    -m        minimized mapping\n"
            "\n"
            "features:\n"
            "    Fn-Q      (0x13d)      --->    KEY_HELP\n"
            "    Fn-<F4>   (0x13e)      --->    KEY_F20 (XF86AudioMicMute)\n"
            "    Fn-<F8>   (0x13f)      --->    KEY_RFKILL\n"
            "    Fn-<SPC>  (0x140-143)  --->    KEY_PROG1-4 (or KEY_BATTERY, XF86KbdLightOnOff for minimized mapping)\n",
            program);
}

int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int opt, mode = 0;

    for (; (opt = getopt(argc, argv, "hm")) != -1;) {
        switch (opt) {
            case 'h':
                return print_usage(stdout, argv[0]), EXIT_SUCCESS;
	    case 'm':
	        mode = 1;
	        continue;
        }
        return print_usage(stderr, argv[0]), EXIT_FAILURE;
    }

    struct input_event input;
    int nextkey = KEY_RESERVED;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN) {
            switch (input.value) {
                case 0x13d:
                    nextkey = KEY_HELP;
                    break;
                case 0x13e:
                    nextkey = KEY_F20; // XF86AudioMicMute
                    break;
                case 0x13f:
                    nextkey = KEY_RFKILL;
                    break;
	        case 0x140: // KEY_BATTERY is XF86KbdLightOnOff
		    nextkey = mode == 1 ? KEY_BATTERY : KEY_PROG1;
                    break;
                case 0x141:
		    nextkey = mode == 1 ? KEY_BATTERY : KEY_PROG2;
                    break;
                case 0x142:
		    nextkey = mode == 1 ? KEY_BATTERY : KEY_PROG3;
                    break;
                case 0x143:
		    nextkey = mode == 1 ? KEY_BATTERY : KEY_PROG4;
                    break;
                default:
                    nextkey = KEY_RESERVED;
            }
            write_event(&input);
            continue;
        }

        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        if (input.code == KEY_UNKNOWN) {
            input.code = nextkey;
            write_event(&input);
            continue;
        } else {
  	    write_event(&input);
	    continue;
	}
    }
}
