package main

 import (
        "database/sql"
      "time"
       _ "github.com/mattn/go-sqlite3"
       "github.com/gin-gonic/gin"
       "fmt"
      "github.com/gin-contrib/cors"
      "log"
	"github.com/gin-gonic/autotls"
   )

func main() {
        db, err := sql.Open("sqlite3","test.db")
        chkErr(err)

        route := gin.Default()
        route.POST("/",  func(c *gin.Context) {

        temperature := c.PostForm("temperature")
        humidity := c.PostForm("humidity")
        pressure := c.PostForm("pressure")
        gas := c.PostForm("gas")
        altitude := c.PostForm("altitude")
        fmt.Println(temperature)

        lengthofTemp := len(temperature)

        if lengthofTemp>0 {
        time := time.Now()
        stmt, err := db.Prepare("INSERT INTO dataset(temperature,humidity,pressure,gas,altitude,time) VALUES(?,?,?,?,?,?) ")
        chkErr(err)

        stmt.Exec(temperature, humidity, pressure, gas, altitude, time)
        chkErr(err)
        c.JSON(200, gin.H{
            "status": "ok",
        })
        } else {
        c.JSON(200, gin.H{
            "status": "no data available",
        })
    }
    
})
  route.GET("/get",  func(c *gin.Context) {

        rows, err := db.Query("SELECT * FROM dataset ORDER BY id DESC Limit 1")
        chkErr(err)

        var id uint
        var temperature string
        var humidity string
        var pressure string
        var gas string
        var altitude string
        var time string

        for rows.Next() {
        err := rows.Scan(&id,&temperature,&humidity,&pressure,&gas,&altitude,&time)
        chkErr(err)

        c.JSON(200, gin.H{
            "temperature" : temperature,
            "humidity" : humidity,
            "pressure" : pressure,
            "gas" : gas,
            "altitude" : altitude,
            "time" : time,  
        }) 
         }   
        rows.Close()
    })
    route.Use(cors.New(cors.Config{
    AllowOrigins:     []string{"*"},
    AllowMethods:     []string{"*"},
    AllowHeaders:     []string{"*"},
    ExposeHeaders:    []string{"*"},
    AllowCredentials: true,
    MaxAge: 12 * time.Hour,
    }))
    log.Fatal(autotls.Run(route, "api.temelkuran.ist"))
}

func chkErr(err error) {
        if err != nil {
        panic(err)
        }
}




