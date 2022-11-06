import { h } from "preact";
import { Link } from "preact-router/match";

import style from "./style.scss";

function Home() {
  return (
    <main id={style.setup} class="container" style={{width: "40%"}}>
      <p>Hello</p>
      <Link href="/setup">Setup</Link>
    </main>
  );
}

export default Home;
