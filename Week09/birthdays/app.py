import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database

        current = {'user_name': request.form['user_name'], 'birth_day': request.form['birth_day'], 'birth_month': request.form['birth_month']}

        query = "INSERT INTO birthdays (name, day, month) VALUES(?, ?, ?)"

        db.execute(query, current['user_name'], current['birth_day'], current['birth_month'])

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html

        query = 'SELECT * FROM birthdays'

        birthdays = db.execute(query)

        return render_template("index.html", birthdays=birthdays)



