NAME        :=	ircserv
CXX         :=	c++
CXXFLAGS    :=	-Wall -Wextra -Werror -fsanitize=address -MMD -std=c++98 -g3

BUILD_DIR   :=	build

SRCS        :=	src/main.cpp \
								src/signals.cpp \
								src/core/Server.cpp \
								src/core/Logger.cpp \
								src/core/ConnectionHandler.cpp \
								src/user/User.cpp \
								src/user/UserManager.cpp \
								src/buffer/Buffer.cpp \
								src/message/Message.cpp

OBJS        := $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
