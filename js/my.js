$(document).ready(()=>{
    $("#btn").click(myFun)
})

function myFun(){
    testo = $("#test").val()
    numero = $("#num").val()

    alert(testo + "---" + numero)
}