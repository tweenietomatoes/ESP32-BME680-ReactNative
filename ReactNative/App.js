/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React from 'react';
import { View, ActivityIndicator } from 'react-native';
import { Container, Header, Left, Body, Right, Title, Text, Button, H1, Content, Badge, Footer, Icon } from 'native-base';
import { Row, Grid } from 'react-native-easy-grid';

export default class App extends React.Component {
   constructor(props){
    super(props);
    this.state ={ isLoading: true, dataSource: null}
  }
 
  componentDidMount(){
    return fetch('https://api.temelkuran.ist/get')
      .then((response) => response.json())
      .then((responseJson) => {

        this.setState({
          isLoading: false,
          dataSource: responseJson,
        }, function(){

        });

      })
      .catch((error) =>{
        console.error(error);
      });
  }

   render(){
    
    if(this.state.isLoading){
      return(
        <Container>
        <H1>App is loading...</H1>
        <ActivityIndicator/>
        </Container>
      );
    }

    return(
       <Container>
      <Header>
     <Left><Text>Y</Text></Left>
     <Body>
       <Title>Höğm App</Title>
     </Body>
     <Right><Text>Y</Text></Right>
   </Header>

   <Content>
<Grid>

<Row>
<Button success><Text>Time: {this.state.dataSource.time}</Text></Button>
</Row>

<Row>
<Button danger>
<Text>{this.state.dataSource.temperature} C</Text>
</Button>

<Button light>
<Text>{this.state.dataSource.pressure} bar</Text>
 </Button>

<Button info>
<Text>{this.state.dataSource.humidity} %rh</Text>
 </Button>

</Row>

<Row>
<Button warning>
<Text>{this.state.dataSource.gas} Kohm</Text>
 </Button>

 <Button dark>
<Text>{this.state.dataSource.altitude} Kohm</Text>
 </Button>
</Row>

</Grid>
</Content>
<Footer><Text><Icon name='home' style={{color: 'white'}}/></Text></Footer>
</Container>
    );
  }
}
