const express = require("express");
const exphbs = require("express-handlebars");
const session = require("express-session");
const passport = require("passport");
const path = require("path");

const app = express();
const port = 3000;

// View engine configuration
const hbs = exphbs.create({
    extname: ".hbs",
    defaultLayout: "layout",
    layoutsDir: path.join(__dirname, "views"),
    // partialsDir: path.join(__dirname, "views/user/partials"),
    helpers: {
        eq: function (v1, v2) {
            return v1 === v2;
        },
        lowercase: function (str) {
            return str.toLowerCase();
        },
        subtract: (price, discount) => {
            return price - discount;
        },
    },
});

app.engine("hbs", hbs.engine);
app.set("view engine", "hbs");

app.use("/", require("./routes/index"));

app.get("/", (req, res) => {
    res.send("Hello, Express!");
});





app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});

// catch 404 and forward to error handler
app.use(function (req, res, next) {
    next(createError(404));
});

module.exports = app;
