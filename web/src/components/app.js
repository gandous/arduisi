import { h } from "preact";
import { Router } from "preact-router";
import "@picocss/pico";

import Home from "routes/home";
import Setup from "routes/setup";

function App() {
	return (
		<div id="app" className="container-fluid" data-theme="dark">
			<Router>
				<Home path="/" />
				<Setup path="/setup" />
			</Router>
		</div>
	);
}

export default App;
