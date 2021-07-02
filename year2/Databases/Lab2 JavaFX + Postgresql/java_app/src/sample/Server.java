package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.sql.*;
import java.util.ArrayList;


public class Server {
    private final Connection connection;
    private final String prefix = "jdbc:postgresql://";
    private final String host_name;
    private final String port;
    private final String username;
    private final String password;
    private final ArrayList<Database> databases = new ArrayList<>();

    Server(String host, String port, String username, String password) throws SQLException {
        this.host_name = host;
        this.port = port;
        this.username = username;
        this.password = password;
        this.connection = DriverManager.getConnection(getUrl(), username, password);
        System.out.println("Connected to: " + getUrl());
        var databasesNames = listDownAllDatabases();
        for (var name : databasesNames) {
            databases.add(new Database(this, name));
        }
    }

    public Connection getConnection() {
        return connection;
    }

    public String getUrl() {
        return prefix + host_name + ":" + port + "/";
    }

    public String getUsername() {
        return username;
    }

    public String getPass() {
        return password;
    }

    public Database getDatabase(String name) throws Exception {
        return databases.stream()
                .filter(x -> x.getName().equals(name))
                .findAny()
                .orElseThrow(() -> new Exception("Database " + name + " not found"));
    }

    public Database createDB(String newDBname) throws SQLException {
        connection.prepareStatement("CREATE DATABASE " + newDBname).execute();
        connection.prepareStatement("CALL create_tables();").execute();
        return new Database(this, newDBname);
    }

    public ObservableList<String> listDownAllDatabases() {
        ObservableList<String> result = FXCollections.observableArrayList();
        try {
            PreparedStatement ps = connection
                    .prepareStatement("SELECT datname FROM pg_database WHERE datistemplate = false;");
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                result.add(rs.getString(1));
            }
            rs.close();
            ps.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }

    public void deleteDB(String dbName) {
        try {
            connection.prepareStatement("DROP DATABASE " + dbName).execute();
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
    }
}
