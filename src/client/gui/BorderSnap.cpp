#include "BorderSnap.hpp"

#include <boost/algorithm/string.hpp>
#include "common/util/Utils.hpp"

void BorderSnap::SnapAllWidgets(MyGUI::Gui* gui, int width, int height) {
    MyGUI::EnumeratorWidgetPtr widgets = gui->getEnumerator();
    while(widgets.next()) {
        MyGUI::Widget* wp = widgets.current();
        if(wp->isUserString("BorderSnap_Borders") && wp->getUserString("BorderSnap_Borders") != "") {
            // enable border snap for wp
            if(wp->isUserString("BorderSnap_Borders")) {
                std::string padding = "0 0 0 0";
                if(wp->isUserString("BorderSnap_Padding"))
                    padding = wp->getUserString("BorderSnap_Padding");

                std::vector<std::string> paddings = split(padding, " ");
                std::vector<std::string> borders = split(wp->getUserString("BorderSnap_Borders"), " ");

                if(borders.size() == 4 && paddings.size() == 4) {
                    if(borders[0] == "true") {
                        // snap left
                        wp->setPosition(toint(paddings[0]), wp->getTop());
                    }
                    if(borders[1] == "true") {
                        // snap top
                        wp->setPosition(wp->getLeft(), toint(paddings[1]));
                    }
                    if(borders[2] == "true") {
                        // snap right
                        if(borders[0] == "true") {
                            //extend
                            wp->setSize(width - toint(paddings[0]) - toint(paddings[2]), wp->getHeight());
                        } else {
                            // move
                            wp->setPosition(width - wp->getWidth() - toint(paddings[2]), wp->getTop());
                        }
                    }
                    if(borders[3] == "true") {
                        // snap bottom
                        if(borders[1] == "true") {
                            //extend
                            wp->setSize(wp->getHeight(), height - toint(paddings[1]) - toint(paddings[3]));
                        } else {
                            // move
                            wp->setPosition(wp->getLeft(), height - wp->getHeight() - toint(paddings[3]));
                        }
                    }
                }
            }
        }
    }

}
