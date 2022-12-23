import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    query = "SELECT buy_date, symbol, company, sum(shares) AS sum_shares FROM orders WHERE person_id = ? GROUP BY symbol"
    orders = db.execute(query, session['user_id'])
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]

    stocks_current_prices = []

    for order in orders:
        current = lookup(order['symbol'])
        stocks_current_prices.append(current['price'] * order['sum_shares'])

    print(stocks_current_prices)
    all_stocks_sum = sum(stocks_current_prices)

    return render_template("index.html", cash=user['cash'], orders=orders, stocks_current_prices=stocks_current_prices, all_stocks_sum=all_stocks_sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol, shares = request.form.get("symbol"), request.form.get("shares")

        if not symbol or not shares:
            return apology("You must fill all fields", 400)

        if not lookup(symbol):
            return apology("We couldn't find that symbol.", 400)

        try:
            shares = int(shares)
        except:
            return apology("The shares field must contain only numbers.", 400)

        if shares < 1:
            return apology("The shares field must contain only positive numbers.", 400)

        stock = lookup(symbol)

        query_user = "SELECT * FROM users WHERE id = ?"
        query_buy_stocks = "INSERT INTO orders(person_id, shares, share_price, buy_date, symbol, company) VALUES (?, ?, ?, ?, ?, ?)"

        user = db.execute(query_user, session["user_id"])[0]

        stocks_sum = stock['price'] * shares

        if user['cash'] >= stocks_sum:
            db.execute(query_buy_stocks, session['user_id'], shares, stock['price'], datetime.now(), symbol, stock['name'])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user['cash'] - stocks_sum, session['user_id'])
        else:
            return apology("You don't have sufficient cash to finish this operation", 400)

        return redirect("/")



    return render_template("buy.html")

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        old_password, new_password, confirmation = request.form.get("current_password"), request.form.get("new_password"), request.form.get("confirmation")
        user_hash_password = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]

        if not old_password or not new_password or not confirmation:
            return apology("All fields must be filled!", 403)

        if not check_password_hash(user_hash_password['hash'], old_password):
            return apology("Invalid old password!", 403)

        if new_password != confirmation:
            return apology("New password and confirmation must be the same!", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new_password), session["user_id"])

        return redirect("/")

    return render_template("change_password.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM orders WHERE person_id = ? ORDER BY buy_date DESC", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("You must fill the 'symbol' field", 400)

        fetch_result = lookup(symbol)

        if not fetch_result:
            return apology("Sorry, we didn't find that symbol.", 400)

        return render_template("quoted.html", fetch_result=fetch_result)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username, password, confirmation = request.form.get("username"), request.form.get("password"), request.form.get("confirmation")

        if not username or not password or not confirmation:
            return apology("You must fill all fields!", 400)

        if password != confirmation:
            return apology("Password and confirmation must be the same!")

        register_query = "INSERT INTO users (username, hash) VALUES (?, ?)"
        hashy_password = generate_password_hash(password)

        try:
            db.execute(register_query, username, hashy_password)
        except(ValueError):
            return apology("That username was already taken", 400)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    query = "SELECT symbol FROM orders WHERE person_id = ? GROUP BY symbol"
    user_symbol = request.form.get("symbol")
    symbols = db.execute(query, session['user_id'])
    symbols_buffer = []

    for current in symbols:
        symbols_buffer.append(current['symbol'])

    if request.method == "POST":
        shares = request.form.get("shares")

        if not user_symbol:
            return apology("You must select a symbol!", 400)

        if user_symbol not in symbols_buffer:
            return apology("You must select a symbol that you have bought!", 400)

        if not shares:
            return apology("You must fill all the fields!", 400)

        if int(shares) < 1:
            return apology("You must fill the shares field with a positive integer!", 400)

        company_stock_price = lookup(user_symbol)["price"]
        user_stocks = db.execute("SELECT sum(shares) AS shares_sum FROM orders WHERE person_id = ? AND symbol = ?", session["user_id"], user_symbol)[0]

        if int(shares) > user_stocks['shares_sum']:
            return apology("You do not own this amount of shares!", 400)

        sell_value = int(shares) * company_stock_price
        sell_query = "INSERT INTO orders(person_id, shares, share_price, buy_date, symbol, company) VALUES (?, ?, ?, ?, ?, ?)"
        company_name = db.execute("SELECT company FROM orders WHERE symbol = ? GROUP BY symbol", user_symbol)[0]

        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]

        db.execute(sell_query, session["user_id"], int(shares) * -1, company_stock_price, datetime.now(), user_symbol, company_name['company'])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user["cash"] + sell_value, session["user_id"])

        return redirect("/")

    return render_template("sell.html", symbols_buffer=symbols_buffer)
