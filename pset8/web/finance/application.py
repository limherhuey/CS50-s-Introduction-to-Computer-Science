import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    cash = db.execute("SELECT cash FROM users WHERE id = :id",
                      id=session["user_id"])
    stocks = db.execute("SELECT symbol, SUM(shares) as n FROM shares WHERE user_id = :id GROUP BY symbol HAVING n > 0",
                        id=session["user_id"])

    quotes, total = {}, {}
    balance = cash[0]["cash"]
    for stock in stocks:
        quotes[stock["symbol"]] = lookup(stock["symbol"])
        total[stock["symbol"]] = quotes[stock["symbol"]]["price"] * stock["n"]
        balance += total[stock["symbol"]]

    return render_template("index.html", stocks=stocks, quotes=quotes, total=total, cash=cash[0]["cash"], balance=balance)


# check if text is an int // helper for buy() and sell()
def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    else:
        if not lookup(request.form.get("symbol")):
            return apology("invalid share symbol")

        elif not is_int(request.form.get("shares")) or int(request.form.get("shares")) < 1:
            return apology("you can only buy a positive integer of shares", 403)

        quote = lookup(request.form.get("symbol"))
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])
        total = quote["price"] * int(request.form.get("shares"))

        if total > rows[0]["cash"]:
            return apology("you do not have enough cash to make this purchase", 403)

        # update shares and users table
        db.execute("INSERT INTO shares (user_id, company, symbol, shares, price, total) VALUES (:id, :name, :symbol, :shares, :price, :total)",
                   id=session["user_id"], name=quote["name"], symbol=quote["symbol"], shares=request.form.get("shares"), price=quote["price"], total=total)
        db.execute("UPDATE users SET cash = cash - :total WHERE id = :id",
                   total=total, id=session["user_id"])

        flash("Bought!")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    transactions = db.execute("SELECT symbol, shares, price, datetime FROM shares WHERE user_id = :id",
                              id=session["user_id"])

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in as " + request.form.get("username"))
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
    if request.method == "GET":
        return render_template("quote.html")

    else:
        if not lookup(request.form.get("symbol")):
            return apology("invalid share symbol")

        quote = lookup(request.form.get("symbol"))
        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    else:
        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif db.execute("SELECT * FROM users WHERE username = :username",
                        username=request.form.get("username")):
            return apology("username has been taken, please choose another username", 403)

        elif not request.form.get("password"):
            return apology("must enter password", 403)

        elif request.form.get("password") != request.form.get("cpassword"):
            return apology("must confirm password")

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                   username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        flash("Welcome!")
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        stocks = db.execute("SELECT symbol FROM shares WHERE user_id = :id GROUP BY symbol HAVING SUM(shares) > 0",
                            id=session["user_id"])
        return render_template("sell.html", stocks=stocks)

    else:
        if not is_int(request.form.get("shares")) or int(request.form.get("shares")) < 1:
            return apology("you can only sell a positive integer of shares")

        shares = db.execute("SELECT SUM(shares) as n FROM shares WHERE user_id = :id AND symbol = :symbol GROUP BY symbol",
                            id=session["user_id"], symbol=request.form.get("symbol"))
        if int(request.form.get("shares")) > shares[0]["n"]:
            return apology("you do not hold this much shares from " + request.form.get("symbol"))

        quote = lookup(request.form.get("symbol"))
        total = quote["price"] * int(request.form.get("shares"))

        # update shares and users table
        db.execute("INSERT INTO shares (user_id, company, symbol, shares, price, total) VALUES (:id, :name, :symbol, :shares, :price, :total)",
                   id=session["user_id"], name=quote["name"], symbol=quote["symbol"], shares=-int(request.form.get("shares")), price=quote["price"], total=total)
        db.execute("UPDATE users SET cash = cash + :total WHERE id = :id",
                   total=total, id=session["user_id"])

        flash("Sold!")
        return redirect("/")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "GET":
        return render_template("change_password.html")

    else:
        user = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])

        if not check_password_hash(user[0]["hash"], request.form.get("oldpassword")):
            return apology("old password is incorrect")

        elif not request.form.get("newpassword"):
            return apology("what is your new password even")

        elif request.form.get("newpassword") != request.form.get("cnewpassword"):
            return apology("please confirm new password")

        db.execute("UPDATE users SET hash = :newhash WHERE id = :id",
                   newhash=generate_password_hash(request.form.get("newpassword")), id=session["user_id"])

        flash("Password changed!")
        return redirect("/")


@app.route("/cheat", methods=["GET", "POST"])
@login_required
def cheat():
    if request.method == "GET":
        return render_template("cheat.html")

    else:
        try:
            float(request.form.get("amount"))
        except ValueError:
            return apology("you must enter a real number")

        if float(request.form.get("amount")) < 0:
            return apology("okay you can't cheat to reduce your cash")

        elif request.form.get("password") != "cheat":
            return apology("wrong password")

        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :id",
                   amount=request.form.get("amount"), id=session["user_id"])

        flash("You cheated to get $" + request.form.get("amount"))
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
