pragma Singleton
import QtQuick 2.0

Item {
    property var locale: Qt.locale()

    function formatDate(date, format) {
        if (format === "MMMM yyyy" || format === "yyyy MMMM") {
            var fmt = (format === "MMMM yyyy") ? "%1 %2" : "%2 %1"
            return fmt.arg(locale.standaloneMonthName(date.getMonth(), Locale.LongFormat))
                          .arg(date.getFullYear())
        }

        return Qt.formatDate(date, format)
    }
}
