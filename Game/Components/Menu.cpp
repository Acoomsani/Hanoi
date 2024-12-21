#include "Menu.h"

Menu::Menu(int width_, int height_) : width(width_), height(height_) {
      using ss = status::Status;
      std::vector<std::string> ops;
      options.insert({"ngm", {1, {0.1f, 0.2f, 0.7f}, {}, ss::released, "New Game"}});
      ops.emplace_back("ngm");
      options.insert({"ogm", {1, {0.688f, 0.251f, 0.159f}, {}, ss::released, "History Games"}});
      ops.emplace_back("ogm");
      options.insert({"rec", {1, {0.228f, 0.731f, 0.109f}, {}, ss::released, "Game Records"}});
      ops.emplace_back("rec");
      options.insert({"des", {1, {0.188f, 0.131f, 0.659f}, {}, ss::released, "Game Description"}});
      ops.emplace_back("des");
      options.insert({"demo", {1, {0.888f, 0.131f, 0.159f}, {}, ss::released, "Presentation"}});
      ops.emplace_back("demo");
      options.insert({"qut", {1, {0.188f, 0.731f, 0.109f}, {}, ss::released, "Quit Game"}});
      ops.emplace_back("qut");
      texts.insert({"title", {1.5, {0.288f, 0.231f, 0.959f}, {}, ss::released, "Tower of Hanoi"}});
      texts.insert({"input", {1.4, {0.288f, 0.231f, 0.959f}, {}, ss::released, "Enter num of Disks:"}});
      texts.insert({"num", {1.4, {0.23f, 0.18f, 0.49f}, {}, ss::released, "Num :   "}});
      float $width = (static_cast<float>(width_) / 2);
      float $height = static_cast<float>(height_) - static_cast<float>(height) * 0.245f;
      float offset = (static_cast<float>(height_) - static_cast<float>(height) * 0.178f * 2) / (static_cast<float>(options.size()) - 1.0f);
      for (int i = 0; i < ops.size(); i++) {
            options.at(ops[i]).position = {$width - options.at(ops[i]).context.length() * 26 / 2, $height - offset * i};
      }
      texts.at("title").position = {$width - (float) texts.at("title").context.length() * 42 / 2, height_ - 100};
      texts.at("input").position = {$width - (float) (texts.at("input").context.length()) * 40 / 2, height_ - 200};
      texts.at("num").position = {$width - (float) (texts.at("num").context.length() + 3) * 42 / 2, height_ - 300};
}

void Menu::renderOptions(const Shader &textShader, const Text &text) {
      using so = status::OptionState;
      for (const auto &item: options) {
            auto &o = item.second;
            text.renderText(textShader, o.context, o.position, o.scale, o.color + so::getLight(o.status));
      }
}

void Menu::renderTitle(const Shader &textShader, const Text &text) {
      auto &title = texts.at("title");
      text.renderText(textShader, title.context, title.position, title.scale, title.color);
}

void Menu::release() {
      using ss = status::Status;
      options.at("ngm").status = ss::released;
      options.at("ogm").status = ss::released;
      options.at("rec").status = ss::released;
      options.at("des").status = ss::released;
      options.at("qut").status = ss::released;
      options.at("demo").status = ss::released;
}

void Menu::renderInput(const Shader &textShader, const Text &text, int val) {
      auto num = texts.at("num");
      auto input = texts.at("input");
      text.renderText(textShader, input.context, input.position, input.scale, input.color);
      text.renderText(textShader, num.context + std::to_string(val), num.position, num.scale, num.color);
}

void Menu::renderContext(const Shader &contextShader, int b_height, const std::vector<Message> &messages) const {
      int hei = height - b_height;
      for (const auto &item: messages) {
            hei -= item.offset1;
            item.text1.renderText(contextShader, item.message1, {item.b_width, hei}, item.scale1, item.color1 + item.light);
            item.text2.renderText(contextShader, item.message2, {item.b_width, hei -= item.offset2}, item.scale2, item.color2 + item.light);
      }
}