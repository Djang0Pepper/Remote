/* Get network list */
document.addEventListener("DOMContentLoaded", function(event) {
    getNetworkList();
});
function getNetworkList() {
    var a = new XMLHttpRequest;
    a.onreadystatechange = function() {
        if (a.readyState == XMLHttpRequest.DONE)
            if (200 == a.status) {
                var b = document.getElementById("ssid");
                v = b.getAttribute("value");
                for (b; b.firstChild;) b.removeChild(b.firstChild);
                try {
                    for (var c = JSON.parse(a.responseText), d = c.networks, e = 0; e < d.length; e++) {
                        var f = document.createElement("option");
                        f.value = d[e].name, f.text = d[e].name, "" != d[e].protection && (f.text += "(*)"), f.text += " db: " + d[e].quality, b.appendChild(f);
                        if (d[e].name == v)
                            f.selected=true;
                    }
                } catch (a) {
                    alert("Erreur de réponse du serveur" + a)
                }
            } else 400 == a.status ? alert("Error 400 demande invalide ") : alert("Erreur de communication avec le serveur")
    }, a.open("GET", "/setup/wifilist", !0), a.send()
};