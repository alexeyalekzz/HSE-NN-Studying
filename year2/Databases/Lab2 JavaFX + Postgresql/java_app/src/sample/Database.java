package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.control.MenuItem;

import java.sql.*;

public class Database {
    private final String name;
    private final Server server;
    private Connection connection;

    Database(Server server, String name) {
        this.name = name;
        this.server = server;
    }

    public void connect() throws SQLException {
        if (connection == null) {
            this.connection = DriverManager.getConnection(server.getUrl() + name, server.getUsername(), server.getPass());
            System.out.println("Connected to: " + server.getUrl() + name);
        }
    }

    public void disconnect() throws SQLException {
        if (connection != null) {
            connection.close();
            connection = null;
            System.out.println("Disconnected from: " + server.getUrl() + name);
        }
    }

    public String getName() {
        return name;
    }

    public Connection getConnection() {
        return connection;
    }

    public Server getServer() {
        return server;
    }

    public ObservableList<MenuItem> getTableNames() throws SQLException {
        ObservableList<MenuItem> result = FXCollections.observableArrayList();
        ResultSet rs =  connection.getMetaData().getTables(null, null, "%", new String[]{"TABLE"});

        while (rs.next()) {
            var name = rs.getString(3);
            result.add(new MenuItem(name));
        }

        return result;
    }

    public ResultSet getTable(String name) throws SQLException {
        return connection.createStatement().executeQuery("SELECT * FROM " + name);
//        return connection.prepareStatement(String.format("SELECT get_rows_test('%s')", name)).executeQuery();
    }

    public void deleteTable(String tableName) throws SQLException {
        System.out.printf("Delete table %s: %b\n",
                tableName,
                connection.prepareStatement(String.format("CALL delete_table('%s')", tableName)).execute());
    }

    public void clearTable(String tableName) throws SQLException {
        System.out.printf("Truncate table %s: %b\n",
                tableName,
                connection.prepareStatement(String.format("CALL clear_table('%s');", tableName)).execute());
    }

    public void addTableRow(String tableName, String[] fields) throws SQLException {
        var t = getTable(tableName);
        String[] arr = new String[t.getMetaData().getColumnCount() - 1];
        for (int i = 1; i < t.getMetaData().getColumnCount(); i++) {
            if (!t.getMetaData().getColumnName(i + 1).equals("ИДЕНТИФИКАТОР")) {
                arr[i - 1] = t.getMetaData().getColumnName(i + 1);
            }
        }

        int j = 0;
        String[] tt = new String[fields.length - 1];
        for (String field : fields) {
            if (field != null) {
                tt[j++] = field;
            }
        }

        String s = String.format("""
                         INSERT INTO %s ("%s") VALUES ('%s');""",
                            tableName,
                            String.join("\", \"", arr),
                            String.join("', '", tt));

        System.out.println(s);
        connection.prepareStatement(s).execute();
    }

    public void clearAllTables() throws SQLException {
        System.out.printf("Truncate all tables: %b\n",
                connection.prepareStatement(String.format("SELECT clear_all_tables('%s')", server.getUsername())).execute());
    }

    public void deleteRow(String tableName, long id) throws SQLException {
        System.out.println(tableName + id);
        System.out.printf("Delete row: %b\n",
                connection.prepareStatement(String.format("CALL delete_record('%s', %d)", tableName, id)).execute());
    }

    public void renameTable(String tableName, String newName) throws SQLException {
        System.out.printf("Rename: %b",
                connection.prepareStatement(String.format("CALL rename_table('%s', '%s');", tableName, newName)).execute());
    }
}
