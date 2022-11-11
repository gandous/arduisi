import { h } from "preact";
import axios from "axios";
import { Link } from "preact-router/match";

import style from "./style.scss";

function Home() {
  return (
    <main id={style.home} class="container">
      <hgroup>
        <h1>Arduisi</h1>
        <h6>Config</h6>
      </hgroup>
      <p>Screen :</p>
      <div class="grid">
        <button onClick={() => axios.post("http://arduisi.local/api/screen-off")}>Turn off</button>
        <button onClick={() => axios.post("http://arduisi.local/api/screen-on")}>Turn on</button>
      </div>
      <Link href="/setup">Setup</Link>
    </main>
  );
}

export default Home;
