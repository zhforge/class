function openSubsection(subsectionName) {
    var i;
    var x = document.getElementsByClassName("subsection");
    for (i = 0; i < x.length; i++) {
      x[i].style.display = "none";
    }
    document.getElementById(subsectionName).style.display = "grid";
}