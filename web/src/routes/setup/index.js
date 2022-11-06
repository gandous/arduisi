import { h } from "preact";
import { useEffect, useState } from "preact/hooks";
import axios from "axios";

import style from "./style.scss";

const State = {
  loading: "loading",
  refreshing: "refreshing",
  connecting: "connecting",
  connected: "connected",
  none: "none"
}

function NetworksLists({networks, onClick}) {
  return (
    <table>
      <tbody>
      {networks.map((value, index) => {
        return (
          <tr onClick={() => onClick(index)} key={value}>
            <td>{value}</td>
            <td>&gt;</td>
          </tr>
          );
        })
      }
      </tbody>
    </table>
  );
}

function PasswordInput({name, onGoBack, onConnect}) {
  const [password, setPassword] = useState("");

  return (
    <>
      <button onClick={onGoBack} class="secondary" style={{width: "7em"}}>go back</button>
      <label>Enter password for "{name}" :</label>
      <input type="password" placeholder="password" value={password} onChange={(evt) => setPassword(evt.target.value)} />
      <button onClick={() => onConnect(password)}>Connect</button>
    </>
  );
}

function Setup() {
  const [networks, setNetworks] = useState([]);
  // const [networks, setNetworks] = useState(["test1", "mon super reseau", "aaaaaa", "ooooooo"]);
  const [selected, setSelected] = useState(-1);
  const [state, setState] = useState(State.loading);
  const [err, setErr] = useState("");
  const controller = new AbortController;

  useEffect(() => {
    updateNetworks();
  }, []);

  useEffect(() => {
    const interval = selected === -1 ? setInterval(() => {
      setState(State.refreshing);
      updateNetworks();
    }, 10000) : undefined;
    return () => {
      interval !== undefined ? clearInterval(interval) : null;
      controller.abort();
      console.log("aboirt");
    };
  }, [selected]);

  const updateNetworks = () => {
    axios.get("http://192.168.4.1/api/network", {signal: controller.signal})
    .then((resp) => {
      setNetworks(resp.data.networks);
      setState(State.none);
    })
  }

  const onNetworkSelected = (index) => {
    console.log(index);
    setSelected(index);
  };

  const onConnect = (password) => {
    console.log("password", password);
    setState(State.connecting);
    axios.post("http://192.168.4.1/api/network", {ssid: networks[selected], password})
    .then(() => {
      setState(State.connected);
    }).catch(() => {
      setErr("Failed to connect");
    })
  }

  const content = () => {
    if (state === State.loading) {
      return (<p class={style.loadingText} aria-busy="true">Loading available network</p>);
    } else if (state === State.connecting) {
      return (<p class={style.loadingText} aria-busy="true">Connecting...</p>);
    } else if (state === State.connected) {
      return (<p class={style.loadingText}>Arduisi is now connected to your local newtwork. You can access it on <a href="arduisi.local">arduisi.local</a></p>);
    } else if (selected === -1) {
      return (
        <div>
          <NetworksLists networks={networks} onClick={onNetworkSelected} />
          {state === State.refreshing ? <p class={style.loadingText} aria-busy="true">Refreshing...</p> : null}
        </div>
      );
    } else {
      return (<PasswordInput onConnect={onConnect} onGoBack={() => setSelected(-1)} name={networks[selected]} />);
    }
  }

  return (
    <main id={style.setup} class="container" style={{width: "40%"}}>
      <hgroup>
        <h1>Arduisi</h1>
        <h6>Setup</h6>
      </hgroup>
      <p>Network :</p>
      <article>
        {content()}
      </article>
    </main>
  );
}

export default Setup;
