#include <radix/screen/XMLScreenLoader.hpp>
#include <radix/env/Util.hpp>
#include <radix/core/math/Vector3f.hpp>

using namespace tinyxml2;

namespace radix {

std::shared_ptr<Screen> XMLScreenLoader::loadScreen(const std::string &path) {
  std::shared_ptr<Screen> screen = std::make_shared<Screen>(); //setup screen pointer

  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str()); //load in XML document

  if (error){
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement("Screen").ToElement();
    XMLHandle rootHandle = XMLHandle(element);

    element->QueryFloatAttribute("alpha", &screen->alpha);
    screen->text = loadText(rootHandle);
    screen->textColor = loadTextColor(rootHandle);
    screen->bgColor = loadbgColor(rootHandle);

    if (screen->alpha == 0) Util::Log(Error, "XMLScreenLoader") << "Failed to find alpha attribute in " << path;
    if (screen->text.empty()) Util::Log(Error, "XMLScreenLoader") << "Failed to find text element in " << path;
    if (screen->textColor.x == 0) Util::Log(Error, "XMLScreenLoader") << "Failed to find text color element in " << path;
    if (screen->bgColor.x == 0) Util::Log(Error, "XMLScreenLoader") << "Failed to find background color element in " << path;

    return screen;
  } else {
    Util::Log(Error, "XMLScreenLoader") << "Failed to load screen " << path;
    return nullptr;
  }
}

std::vector<Text> XMLScreenLoader::loadText(XMLHandle &rootHandle) {
  std::vector<Text> text;
  XMLElement *currElement = rootHandle.FirstChildElement("TextSection").FirstChildElement().ToElement(); //grab the first element under the text section
  if (currElement){
    do {
      Text tempText{};

      currElement->QueryFloatAttribute("x", &tempText.position.x);
      currElement->QueryFloatAttribute("y", &tempText.position.y);
      currElement->QueryFloatAttribute("z", &tempText.position.z);
      currElement->QueryFloatAttribute("size", &tempText.size);
      tempText.text = currElement->GetText();

      text.push_back(tempText);
    } while((currElement = currElement->NextSiblingElement()) != nullptr);
  }

  return text;
}

Vector4f XMLScreenLoader::loadTextColor(tinyxml2::XMLHandle &rootHandle) {
  Vector4f color;
  XMLElement *currElement = rootHandle.FirstChildElement("ColorSection").FirstChildElement("TextColor").ToElement();

  if (currElement) {
    currElement->QueryFloatAttribute("r", &color.x);
    currElement->QueryFloatAttribute("g", &color.y);
    currElement->QueryFloatAttribute("b", &color.z);
    currElement->QueryFloatAttribute("a", &color.w);
  }

  return color;
}

Vector4f XMLScreenLoader::loadbgColor(tinyxml2::XMLHandle &rootHandle) {
  Vector4f color;
  XMLElement *currElement = rootHandle.FirstChildElement("ColorSection").FirstChildElement("BgColor").ToElement();

  if (currElement) {
    currElement->QueryFloatAttribute("r", &color.x);
    currElement->QueryFloatAttribute("g", &color.y);
    currElement->QueryFloatAttribute("b", &color.z);
    currElement->QueryFloatAttribute("a", &color.w);
  }

  return color;
}
}
