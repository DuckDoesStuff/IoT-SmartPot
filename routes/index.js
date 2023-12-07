const express = require("express");
const router = express.Router();
const bodyParser = require("body-parser");
const mqtt = require("mqtt");

const options = {
    protocol: "mqtts",
    host: "broker.hivemq.com",
    port: 1883,
};

const client = mqtt.connect(options);

router.use(bodyParser.json());

router.get("/", async function (req, res, next) {
    res.render("index");
});

router.post("/buttonClick", async (req, res, next) => {
    if (req.body.state) {
        client.publish("tienduc/123/led", "on");
    } else {
        client.publish("tienduc/123/led", "off");
    }
    next();
});

module.exports = router;
