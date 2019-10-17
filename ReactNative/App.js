/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React from 'react';
import {
  View,
  ActivityIndicator
} from 'react-native';
import { Container, Header, Left, Body, Right, Title, Text, Button } from 'native-base';
import { Row, Grid } from 'react-native-easy-grid';

export default class App extends React.Component {
   constructor(props){
    super(props);
    this.state ={ isLoading: true}
  }

  //abortController = new AbortController();

  componentDidMount(){
    return fetch('https://api.temelkuran.ist/get')
      .then((response) => response.json())
      .then((responseJson) => {

        this.setState({
          isLoading: false,
          dataSource: responseJson,
        }, function(){

        });

      }).catch((error) => {
        console.error(error);
      });
    
  }
/*
, {signal: this.abortController.signal}

  .catch((error) =>{
        if (error.name === 'AbortError') return
        throw error
      });

  componentWillUnmount() {
    this.abortController.abort();
  }
*/
   render(){

    if(this.state.isLoading){
      return(
        <View style={{flex: 1, padding: 20}}>
          <Text>Fetching...</Text><ActivityIndicator/>
        </View>
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
<Grid>
 

 <Row size={5} />
  <Row size={75}>
<Button danger>
<Text>{this.state.dataSource.temperature} C</Text>
</Button>

<Button light>
<Text>{this.state.dataSource.pressure} bar</Text>
 </Button>

<Button info>
<Text>{this.state.dataSource.humidity} %rh</Text>
 </Button>

<Button warning>
<Text>{this.state.dataSource.gas} Kohm</Text>
 </Button>

</Row>
</Grid>
</Container>
    );
  }
}
