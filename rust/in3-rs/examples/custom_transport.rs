extern crate in3;

use in3::prelude::*;

fn main() {
    let mut c = Client::new(ChainId::Mainnet);
    c.set_auto_update_nodelist(false);
    c.set_transport(Box::new(|payload: &str, urls: &[&str]| {
        let mut responses = vec![];
        responses.push(Ok(r#"{"jsonrpc":"2.0","id":1,"result":"0x948f0d","in3":{"lastValidatorChange":0,"lastNodeList":9698978,"execTime":454,"rpcTime":454,"rpcCount":1,"currentBlock":9735949,"version":"2.1.0"}}"#.to_string()));
        responses
    }));
    match c.rpc(r#"{"method": "eth_blockNumber", "params": []}"#) {
        Ok(res) => println!("{}", res),
        Err(err) => println!("{}", err)
    }
}