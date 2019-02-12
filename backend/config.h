#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <list>

class config {
    public:
        bool setup(int ac, char **av);
        void dump_config();

        int server_port() const { return server_port_; }

    private:
        void set_defaults();
        bool read_config();
        bool verify_config();

        std::string cfg_filename_;
        int server_port_;
};

#endif /* CONFIG_H */
