import { h } from "preact";
import axios from "axios";
import { Link } from "preact-router/match";
import { useEffect, useState } from "preact/hooks";
import { route } from 'preact-router';

import style from "./style.scss";

function Content() {
  return (
    <>
      <p>Screen :</p>
      <div class="grid">
        <button onClick={() => axios.post("/api/screen-off")}>Turn off</button>
        <button onClick={() => axios.post("/api/screen-on")}>Turn on</button>
      </div>
      <Link href="/setup">Setup wifi</Link>
    </>
  );
}

function Home() {
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    axios.get("/api/status")
    .then((resp) => {
      setLoading(false);
      if (resp.data.failedToConnect == true) {
        console.log("Failed to connect. Navigating to setup");
        route("/setup");
      }
    }).catch(_ => {
      setLoading(false);
    });
  }, []);

  return (
    <main id={style.home} class="container">
      <hgroup>
        <h1>Arduisi</h1>
        <h6>Config</h6>
      </hgroup>
      {loading === true ? <p aria-busy="true">Loading...</p> : <Content />}
    </main>
  );
}

export default Home;
