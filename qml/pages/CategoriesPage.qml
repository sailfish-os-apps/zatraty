import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.zatraty 1.0

Page {
    id: categoriesPage

    SilicaListView {
        id: listView

        PullDownMenu {
            MenuItem {
                text: qsTr("Add category")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../components/NewCategoryDialog.qml"))
                    dialog.accepted.connect(function() {
                        categoryModel.add(dialog.name)
                    })
                }
            }
        }

        model: CategoryListModel {
            id: categoryModel
        }

        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Categories")
        }

        delegate: ListItem {
            id: delegate
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Rename")
                    onClicked: {
                        var category = categoryModel.get(index)
                        var dialog = pageStack.push(Qt.resolvedUrl("../components/NewCategoryDialog.qml"),
                                                    { "name": category.name })
                        dialog.accepted.connect(function() {
                            category.name = dialog.name
                            categoryModel.refresh()
                        })
                    }
                }
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: {
                        var category = categoryModel.get(index)
                        var dialog = pageStack.push(Qt.resolvedUrl("../components/DeleteCategoryDialog.qml"),
                                                    { "category": category })
                        dialog.accepted.connect(function() {
                            categoryModel.remove(index)
                        })
                    }
                }
            }

            Label {
                x: Theme.paddingLarge
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                width: parent.width - 2 * Theme.paddingLarge
                truncationMode: TruncationMode.Elide
                elide: Text.ElideRight
            }

            onClicked: {
                var category = categoryModel.get(index)
                pageStack.push(Qt.resolvedUrl("CategoryPage.qml"), { "category": category })
            }
        }
        VerticalScrollDecorator {}
    }
}
