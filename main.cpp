#include <QCoreApplication>
#include <QtSql>

//Method create connected with DB
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName ("test_database");
    db.setHostName ("127.0.0.1");
    db.setUserName ("database_user");
    db.setPassword ("password");

    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}


int main(int c, char ** v)
{
    QCoreApplication app(c, v);

    if (!createConnection()) {
        return -1;
    }

    QSqlQuery query;

    // Drop data base
    QString str_drop = "DROP TABLE azimuth_log;";
    if (!query.exec(str_drop)) {
        qDebug() << "Table in not created";
    }

    //Creating of the data base
    QString   str  = "CREATE TABLE azimuth_log ( "
        "log_date DATE NOT NULL, "
        "log_time TIME NOT NULL, "
        "azimuth_val INT NOT NULL, "
        "azimuth_val_minutes INT NOT NULL, "
        "azimuth_val_seconds REAL NOT NULL, "
        "sko REAL NOT NULL, "
        "mode INT NOT NULL, "
        "filename CHAR(255) NOT NULL "
    ");";

    if (!query.exec(str)) {
        qDebug() << "Unable to create a table";
    }

    //Adding some information
    QString strF =
          "INSERT INTO  azimuth_log (log_date, log_time, azimuth_val, azimuth_val_minutes, azimuth_val_seconds, sko, mode, filename) "
          "VALUES(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8');";

    str = strF.arg(QDate::currentDate ().toString ("yyyy-MM-dd"))
              .arg(QTime::currentTime ().toString ("HH:mm:ss"))
              .arg (0)
              .arg (0)
              .arg (0)
              .arg (0)
              .arg (1)
              .arg("nofile");


    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation";
    }

    str = strF.arg(QDate::currentDate ().toString ("yyyy-MM-dd"))
               .arg(QTime::currentTime ().toString ("HH:mm:ss"))
               .arg (0)
               .arg (0)
               .arg (0)
               .arg (0)
               .arg (0)
               .arg("nofile");

    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation";
    }

    if (!query.exec("SELECT * FROM azimuth_log;")) {
        qDebug() << "Unable to execute query - exiting";
        return 1;
    }

    //Reading of the data
    QSqlRecord rec     = query.record();
    QDate log_date;
    QTime log_time;
    int azimuth_val = 0;
    int azimuth_val_minutes = 0;
    float_t azimuth_val_seconds = 0;
    float_t sko = 0;
    int mode =0;
    QString filename;



    while (query.next()) {
        log_date  = query.value(rec.indexOf("log_date")).toDate ();
        log_time  = query.value(rec.indexOf("log_time")).toTime ();
        azimuth_val = query.value(rec.indexOf("azimuth_val")).toInt ();
        azimuth_val_minutes = query.value(rec.indexOf("azimuth_val_minutes")).toInt ();
        azimuth_val_seconds  = query.value(rec.indexOf("azimuth_val_seconds")).toFloat ();
        sko  = query.value(rec.indexOf("sko")).toFloat ();
        mode = query.value(rec.indexOf("mode")).toInt ();
        filename = query.value(rec.indexOf("filename")).toString ();

        qDebug() << log_date << " " << log_time << ";\t"
                 << azimuth_val << " " <<  azimuth_val_minutes << " " << azimuth_val_seconds <<";\t"
                 << sko <<" "<< mode << ";\t" << filename;
    }


    return app.exec ();
}

