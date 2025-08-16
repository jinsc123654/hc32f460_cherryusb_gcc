// 包含头文件
#include "string.h"
#include "csh.h"
#include "SEGGER_RTT_APP.h"
#include "stdbool.h"

// 创建一个shell实例
static chry_shell_t csh;
static volatile bool login = true;

// __attribute__((weak)) char __fsymtab_start;
// __attribute__((weak)) char __fsymtab_end;
// __attribute__((weak)) char __vsymtab_start;
// __attribute__((weak)) char __vsymtab_end;

extern const chry_syscall_t __fsymtab_start;
extern const chry_syscall_t __fsymtab_end;
extern const int __vsymtab_start;
extern const int __vsymtab_end;

static int print_all_cmd(void);

// 字符输出函数 -> RTT
static uint16_t csh_sput_cb(chry_readline_t *rl, const void *data, uint16_t size)
{
    (void)rl;
    // 直接把数据写入 RTT 通道 0
    unsigned written = SEGGER_RTT_Write(0, data, size);
    return (uint16_t)written;
}

// 字符输入函数 -> RTT
static uint16_t csh_sget_cb(chry_readline_t *rl, void *data, uint16_t size)
{
    (void)rl;
    uint16_t i;
    for (i = 0; i < size; i++) {
        int ch = SEGGER_RTT_GetKey();
        if (ch < 0) {
            break;  // 没有数据
        }
        ((uint8_t *)data)[i] = (uint8_t)ch;
    }
    return i;
}


int chry_shell_port_hash_strcmp(const char *hash, const char *str)
{
    /*!< simple demo */
    return strcmp(hash, str);
}


// shell 初始化
int shell_init(void)
{
    // 结构体用于配置初始化参数
    chry_shell_init_t csh_init;

    // 设置字符输入输出函数（必须实现）
    csh_init.sput = csh_sput_cb;
    csh_init.sget = csh_sget_cb;

    // linkscript 中定义的符号，用于存放导出的命令和变量


    // 配置函数表和变量表的起始和结束地址（必须实现）
    csh_init.command_table_beg = (chry_syscall_t *)&__fsymtab_start;
    csh_init.command_table_end = (chry_syscall_t *)&__fsymtab_end;
    csh_init.variable_table_beg = (int *)&__vsymtab_start;
    csh_init.variable_table_end = (int *)&__vsymtab_end;

    // 定义一个提示符缓冲区
    static char csh_prompt_buffer[128];
    csh_init.prompt_buffer = csh_prompt_buffer;
    csh_init.prompt_buffer_size = sizeof(csh_prompt_buffer);

    // 定义历史记录缓冲区
    static char csh_history_buffer[128];
    csh_init.history_buffer = csh_history_buffer;
    csh_init.history_buffer_size = sizeof(csh_history_buffer);

    // 定义行输入缓冲区
    static char csh_line_buffer[128];
    csh_init.line_buffer = csh_line_buffer;
    csh_init.line_buffer_size = sizeof(csh_line_buffer);

    // 用户配置
    csh_init.uid = 0;              // 默认用户ID
    csh_init.user[0] = "jsc";      // 用户名
    csh_init.hash[0] = "123";      // 密码哈希
    csh_init.host = "msh";         // Host 名称

    // 初始化 shell
    int ret = chry_shell_init(&csh, &csh_init);
    if (ret) {
        return -1;
    }

    // 注意：不要在这里手动调用 print_all_cmd()
    print_all_cmd();
    return 0;
}


// shell 的主函数，需要再while(1)中循环调用
int shell_main(void)
{
    int ret;

    restart:

        if (login) {
            goto restart2;
        }

    ret = csh_login(&csh);
    if (ret == 0) {
        login = true;
    } else {
        return 0;
    }

    restart2:
        chry_shell_task_exec(&csh);

    ret = chry_shell_task_repl(&csh);

    if (ret == -1) {
        /*!< error */
        return -1;
    } else if (ret == 1) {
        /*!< continue */
        return 0;
    } else {
        /*!< restart */
        goto restart;
    }

    return 0;
}


// Implement the export of a command
// write_led 1 - Turn on the LED
// write_led 0 - Turn off the LED
static int write_led(int argc, char **argv)
{
    (void) argv;

    if (argc < 2) {
        print_rtt("usage: write_led <status>\r\n\r\n");
        print_rtt("  status    0 or 1\r\n\r\n");
        return -1;
    }

    return 0;
}
CSH_CMD_EXPORT(write_led, "?")

// help 命令函数
static int print_all_cmd(void)
{

    const chry_syscall_t *cmd = &__fsymtab_start; print_rtt("\r\nAvailable commands:\r\n"); print_rtt("-------------------\r\n"); while (cmd < &__fsymtab_end) { print_rtt("%-16s : %s\r\n", cmd->name, cmd->help ? cmd->help : "No description"); cmd++; } print_rtt("\r\n"); return 0;
}


#define __ENV_PATH "/sbin:/bin"
const char ENV_PATH[] = __ENV_PATH;
CSH_RVAR_EXPORT(ENV_PATH, PATH, sizeof(__ENV_PATH))
