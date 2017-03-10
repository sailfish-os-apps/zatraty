pragma Singleton
import QtQuick 2.0

Item {
    property var locale: Qt.locale()

    function formatDate(date, format) {
        if (format === "MMMM yyyy") {
            return "%1 %2".arg(locale.standaloneMonthName(date.getMonth(), Locale.LongFormat))
                          .arg(date.getFullYear())
        }

        return Qt.formatDate(date, format)
    }
}
