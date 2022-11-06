import { h } from "preact";
import { Router } from "preact-router";

// Code-splitting is automated for `routes` directory
import Home from "routes/home";
import Profile from "routes/profile";
import Setup from "routes/setup";

function App() {
	return (
		<div id="app" className="container-fluid" data-theme="dark">
			{/* <Header /> */}
			<Router>
				<Home path="/" />
				<Profile path="/profile/" user="me" />
				<Profile path="/profile/:user" />
				<Setup path="/setup" />
			</Router>
		</div>
	);
}

export default App;
