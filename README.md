# Gradebook Application

## Overview

The Gradebook application is a Windows desktop program for managing students, teachers, classes, subjects, and grades. It is built with C++ (MFC) and uses a SQL Server database for data storage.

---

## Prerequisites

- **Windows 10/11**
- **Visual Studio 2022** (with "Desktop development with C++" workload)
- **SQL Server** (Express, Developer, or Standard edition)
- **ODBC Driver 17 for SQL Server** (or compatible)

---

## Database Setup

1. **Locate the `script.sql` File:**
   - The `script.sql` file is included in the project directory. It creates the database and all required tables.

2. **Create the Database:**
   - Open SQL Server Management Studio (SSMS) or your preferred SQL tool.
   - Connect to your SQL Server instance.
   - Open the `script.sql` file.
   - Execute the script. This will:
     - Create a new database (e.g., `GradebookDB`)
     - Create all necessary tables and relationships

   > **Note:** If you want to use a different database name, edit the `CREATE DATABASE` line in `script.sql` and use the same name in your connection string.

---

## Database Connection Configuration

Before running the application, you **must** set the `GRADEBOOK_CONNECTION_STRING` environment variable to your SQL Server connection string.  
The application reads this environment variable at runtime for all database connections.

---

**If running from Command Prompt (temporary for session):**
   set GRADEBOOK_CONNECTION_STRING=Driver={ODBC Driver 17 for SQL Server};Server=YOUR_SERVER;Database=GradebookDB;Trusted_Connection=Yes;

**If running from PowerShell (temporary for session):**
   $env:GRADEBOOK_CONNECTION_STRING="Driver={ODBC Driver 17 for SQL Server};Server=YOUR_SERVER;Database=GradebookDB;Trusted_Connection=Yes;"

**If running from Visual Studio (recommended for debugging):**


   Right-click the Gradebook project in Solution Explorer, select Properties.

   Go to Configuration Properties → Debugging.

   In the Environment field, enter:

   GRADEBOOK_CONNECTION_STRING=Driver={ODBC Driver 17 for SQL Server};Server=YOUR_SERVER;Database=GradebookDB;Trusted_Connection=Yes;
   Click OK to save.

   Replace YOUR_SERVER with your actual SQL Server instance name (e.g., .\SQLEXPRESS).

## Building the Application

1. **Clone or Download the Source Code.**
2. **Open the Solution:**
   - Open `Gradebook.sln` in Visual Studio 2022.
3. **Build the Solution:**
   - Select `Debug` or `Release` configuration.
   - Build the solution (`Build > Build Solution` or press `Ctrl+Shift+B`).

---

## Running the Application

1. **Set the Environment Variable:**
   - Ensure `GRADEBOOK_CONNECTION_STRING` is set in your environment before launching the application.
2. **Start the Application:**
   - Press `F5` or select `Debug > Start Debugging` in Visual Studio.
3. **Begin Using the Application:**
   - Add teachers, classes, students, subjects, and grades using the provided dialogs.

---

## Troubleshooting

- **Database Connection Errors:**  
  Ensure SQL Server is running, the database exists, and the `GRADEBOOK_CONNECTION_STRING` environment variable is set correctly.
- **ODBC Driver Issues:**  
  Install the latest [ODBC Driver for SQL Server](https://learn.microsoft.com/en-us/sql/connect/odbc/download-odbc-driver-for-sql-server).
- **Missing Tables/Columns:**  
  Make sure you have executed the full `script.sql` file.

---